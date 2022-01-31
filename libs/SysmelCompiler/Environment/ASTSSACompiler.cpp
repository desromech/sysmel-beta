#include "Environment/ASTSSACompiler.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

#include "Environment/ASTAnalysisEnvironment.hpp"

#include "Environment/ASTNode.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTCleanUpScopeNode.hpp"
#include "Environment/ASTClosureNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTLexicalScopeNode.hpp"
#include "Environment/ASTMakeAggregateNode.hpp"
#include "Environment/ASTMakeTupleNode.hpp"
#include "Environment/ASTMakeVariantNode.hpp"
#include "Environment/ASTMakeVectorNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTQuoteNode.hpp"
#include "Environment/ASTSequenceNode.hpp"
#include "Environment/ASTVectorSwizzleNode.hpp"

#include "Environment/ASTQuasiQuotePatternExpansionNode.hpp"

#include "Environment/ASTGlobalVariableNode.hpp"
#include "Environment/ASTLocalVariableNode.hpp"
#include "Environment/ASTFieldVariableAccessNode.hpp"
#include "Environment/ASTVariableAccessNode.hpp"
#include "Environment/ASTLocalImmutableAccessNode.hpp"
#include "Environment/ASTSlotAccessNode.hpp"

#include "Environment/ASTFunctionalNode.hpp"
#include "Environment/ASTNamespaceNode.hpp"
#include "Environment/ASTTypeNode.hpp"
#include "Environment/ASTProgramEntityExtensionNode.hpp"

#include "Environment/ASTReinterpretCastNode.hpp"
#include "Environment/ASTValueAsVoidTypeConversionNode.hpp"
#include "Environment/ASTUpcastTypeConversionNode.hpp"
#include "Environment/ASTDowncastTypeConversionNode.hpp"
#include "Environment/ASTValueAsReferenceReinterpretConversionNode.hpp"

#include "Environment/ASTIfNode.hpp"
#include "Environment/ASTWhileNode.hpp"
#include "Environment/ASTDoWhileNode.hpp"
#include "Environment/ASTReturnNode.hpp"

#include "Environment/ASTEvaluatePatternWithValueNode.hpp"
#include "Environment/ASTFailPatternNode.hpp"
#include "Environment/ASTTrapNode.hpp"

#include "Environment/CompiledMethod.hpp"
#include "Environment/SSAFunction.hpp"
#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSACodeRegionArgument.hpp"
#include "Environment/SSACodeRegionCapture.hpp"
#include "Environment/SSABasicBlock.hpp"
#include "Environment/SSABuilder.hpp"

#include "Environment/SSAConstantLiteralValue.hpp"

#include "Environment/SSABreakInstruction.hpp"
#include "Environment/SSACallInstruction.hpp"
#include "Environment/SSAContinueInstruction.hpp"
#include "Environment/SSADoWithCleanupInstruction.hpp"
#include "Environment/SSADoWhileInstruction.hpp"
#include "Environment/SSAGetAggregateFieldReferenceInstruction.hpp"
#include "Environment/SSAGetAggregateSlotReferenceInstruction.hpp"
#include "Environment/SSAIfInstruction.hpp"
#include "Environment/SSALoadInstruction.hpp"
#include "Environment/SSALocalVariableInstruction.hpp"
#include "Environment/SSAMakeClosureInstruction.hpp"
#include "Environment/SSAMakeVectorInstruction.hpp"
#include "Environment/SSAReturnFromFunctionInstruction.hpp"
#include "Environment/SSASendMessageInstruction.hpp"
#include "Environment/SSAStoreInstruction.hpp"
#include "Environment/SSAUnreachableInstruction.hpp"
#include "Environment/SSAVectorSwizzleInstruction.hpp"
#include "Environment/SSAWhileInstruction.hpp"

#include "Environment/SSABitcastInstruction.hpp"
#include "Environment/SSADowncastInstruction.hpp"
#include "Environment/SSAUpcastInstruction.hpp"

#include "Environment/SSAEvaluatePatternInstruction.hpp"
#include "Environment/SSAFailPatternInstruction.hpp"
#include "Environment/SSATrapInstruction.hpp"

#include "Environment/FunctionalType.hpp"
#include "Environment/AggregateType.hpp"
#include "Environment/AggregateTypeLayout.hpp"
#include "Environment/AggregateTypeVariantLayout.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/ArgumentVariable.hpp"
#include "Environment/LocalVariable.hpp"
#include "Environment/VariantType.hpp"

#include "Environment/ControlFlowUtilities.hpp"
#include "Environment/Wrappers.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTSSACompiler> ASTSSACompilerTypeRegistration;

SSAValuePtr ASTSSACompiler::visitNodeForValue(const ASTNodePtr &node)
{
    auto oldPosition = builder->getSourcePosition();
    builder->setSourcePosition(node->sourcePosition);
    auto value = doWithEnsure([&](){
        return visitNode(node);
    }, [&]{
        builder->setSourcePosition(oldPosition);
    });

    sysmelAssert(value->isSSAValue());
    return staticObjectCast<SSAValue> (value);
}

void ASTSSACompiler::assignInitialValueFrom(const SSAValuePtr &destination, const TypePtr &destinationValueType, const SSAValuePtr &initialValue, bool isAggregateComponent)
{
    if(destinationValueType->isPaddingType())
        return;

    auto initialValueType = initialValue->getValueType()->asUndecoratedType();
    auto undecoratedDestinationValueType = destinationValueType->asUndecoratedType();
    if(undecoratedDestinationValueType == initialValueType)
    {
        builder->storeValueIn(initialValue, destination);
    }
    else
    {
        sysmelAssert(initialValueType->isReferenceLikeType());
        if(initialValueType->isTemporaryReferenceType())
        {
            if(!undecoratedDestinationValueType->hasTrivialInitializationMovingFrom())
            {
                auto method = undecoratedDestinationValueType->getInitializeMovingFromMethod()->asSSAValueRequiredInPosition(builder->getSourcePosition());
                auto methodType = method->getValueType();
                sysmelAssert(methodType->isFunctionalType());
                builder->call(methodType.staticAs<FunctionalType> ()->getResultType(), method, {destination, initialValue});
            }
            else
            {
                // Direct copy.
                builder->storeValueIn(builder->load(initialValue), destination);
            }
        }
        else //if(initialValueType->isReferenceType())
        {
            if(!undecoratedDestinationValueType->hasTrivialInitializationCopyingFrom())
            {
                auto method = undecoratedDestinationValueType->getInitializeCopyingFromMethod()->asSSAValueRequiredInPosition(builder->getSourcePosition());
                auto methodType = method->getValueType();
                sysmelAssert(methodType->isFunctionalType());
                builder->call(methodType.staticAs<FunctionalType> ()->getResultType(), method, {destination, initialValue});
            }
            else
            {
                // Direct copy.
                builder->storeValueIn(builder->load(initialValue), destination);
            }
        }
    }

    if(!isAggregateComponent)
        addFinalizationFor(destination, undecoratedDestinationValueType);
}

void ASTSSACompiler::addFinalizationFor(const SSAValuePtr &localVariable, const TypePtr &valueType)
{
    if(valueType->hasTrivialFinalization())
        return;

    localVariable->markLocalFinalizationRequired();
    auto finalizationRegion = builder->makeCodeRegionWithSignature({}, Type::getVoidType());
    buildRegionForSourcePositionWith(finalizationRegion, builder->getSourcePosition(), [&](){
        auto finalizer = valueType->getFinalizeMethod()->asSSAValueRequiredInPosition(builder->getSourcePosition());
        auto finalizerType = finalizer->getValueType();
        sysmelAssert(finalizerType->isFunctionalType());
        auto cleanUp = builder->call(finalizerType.staticAs<FunctionalType> ()->getResultType(), finalizer, {localVariable});
        builder->returnFromRegion(builder->literal(getVoidConstant()));
    });

    builder->enableLocalFinalization(localVariable);
    cleanUpBuilder->localFinalization(localVariable, finalizationRegion);
}

SSAValuePtr ASTSSACompiler::makeAggregateWithElements(const AggregateTypePtr &aggregateType, const SSAValuePtrList &elements)
{
    auto result = builder->localVariable(aggregateType->tempRef(), aggregateType);
    auto layout = aggregateType->getLayout();

    for(size_t i = 0; i < elements.size(); ++i)
    {
        auto slotType = layout->getTypeForNonPaddingSlot(i);
        auto slotIndex = layout->getIndexForNonPaddingSlot(i);
        auto slot = builder->getAggregateSlotReference(slotType->tempRef(), result, builder->literal(wrapValue(slotIndex)));
        assignInitialValueFrom(slot, slotType, elements[i], true);
    }
    
    addFinalizationFor(result, aggregateType);
    return result;
}

void ASTSSACompiler::compileMethodBody(const CompiledMethodPtr &method, const SSAFunctionPtr &ssaFunction, const ASTNodePtr &node, const ASTAnalysisEnvironmentPtr &nodeEnvironment)
{
    currentMethod = method;
    currentSSAFunction = ssaFunction;

    auto methodCodeRegion = ssaFunction->getMainCodeRegion();

    auto mainCodeRegion = basicMakeObject<SSACodeRegion> ();
    mainCodeRegion->setSignature({}, Type::getVoidType());

    auto mainCleanUpCodeRegion = basicMakeObject<SSACodeRegion> ();
    mainCleanUpCodeRegion->setSignature({}, Type::getVoidType());

    currentCodeRegion = mainCodeRegion;
    currentCleanUpCodeRegion = mainCleanUpCodeRegion;

    builder = basicMakeObject<SSABuilder> ();
    builder->setSourcePosition(node->sourcePosition);
    builder->setLexicalScope(nodeEnvironment->lexicalScope);
    builder->setCodeRegion(currentCodeRegion);
    builder->makeBasicBlockHere();

    cleanUpBuilder = basicMakeObject<SSABuilder> ();
    cleanUpBuilder->setSourcePosition(node->sourcePosition);
    cleanUpBuilder->setLexicalScope(nodeEnvironment->lexicalScope);
    cleanUpBuilder->setCodeRegion(currentCleanUpCodeRegion);
    cleanUpBuilder->makeBasicBlockHere();

    auto functionalType = method->getFunctionalType();
    auto receiverType = functionalType->getReceiverType();
    const auto &methodCodeRegionArguments = methodCodeRegion->getArguments();

    size_t receiverOffset = functionalType->getResultType()->isReturnedByReference() ? 1 : 0;
    size_t argumentsOffset = receiverOffset;

    // Map the receiver
    if(!receiverType->isVoidType())
    {
        mapLocalVariableToValue(method->getReceiverArgument(), methodCodeRegionArguments[receiverOffset]);
        ++argumentsOffset;
    }

    // Map the arguments.
    const auto &methodArguments = method->getArguments();
    for(size_t i = 0; i < methodArguments.size(); ++i)
        mapLocalVariableToValue(methodArguments[i], methodCodeRegionArguments[argumentsOffset + i]);

    // Map the captures.
    const auto &methodCaptures = method->getCapturedVariables();
    const auto &mainCodeRegionCaptures = methodCodeRegion->getCaptures();
    for(size_t i = 0; i < methodCaptures.size(); ++i)
        mapLocalVariableToValue(methodCaptures[i], mainCodeRegionCaptures[i]);
    
    auto resultValue = visitNodeForValue(node);
    if(!builder->isLastTerminator())
        returnValueFromFunction(resultValue);

    if(!cleanUpBuilder->isLastTerminator())
        cleanUpBuilder->returnFromRegion(cleanUpBuilder->literal(getVoidConstant()));

    builder->setCodeRegion(methodCodeRegion);
    builder->makeBasicBlockHere();
    builder->doWithCleanUp(mainCodeRegion, mainCleanUpCodeRegion);
    builder->unreachableInstruction();
}

void ASTSSACompiler::mapLocalVariableToValue(const AnyValuePtr &binding, const SSAValuePtr &value)
{
    localVariableMap.insert({binding, value});
}

SSAValuePtr ASTSSACompiler::findLocalVariableMapping(const AnyValuePtr &binding)
{
    auto it = localVariableMap.find(binding);
    return it != localVariableMap.end() ? it->second : nullptr;
}

AnyValuePtr ASTSSACompiler::visitCallNode(const ASTCallNodePtr &node)
{
    // Return by reference hidden pointer.
    auto &resultType = node->analyzedType;
    auto isReturningByReference = resultType->isReturnedByReference();

    auto functionValue = visitNodeForValue(node->function);
    SSAValuePtrList arguments;
    arguments.reserve((isReturningByReference ? 1 : 0) + node->arguments.size());

    // Hidden result pointer.
    SSAValuePtr resultByReference;
    if(isReturningByReference)
    {
        resultByReference = builder->localVariable(resultType->tempRef(), resultType);
        arguments.push_back(resultByReference);
    }

    for(auto &arg : node->arguments)
        arguments.push_back(visitNodeForValue(arg));

    // Perform the call.
    auto callResult = builder->call(node->analyzedType, functionValue, arguments);
    
    // Implicit finalization of result.
    if(isReturningByReference)
    {
        addFinalizationFor(resultByReference, resultType);
        return resultByReference;
    }

    return callResult;
}

AnyValuePtr ASTSSACompiler::visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node)
{
    auto bodyRegion = builder->makeCodeRegionWithSignature({}, node->analyzedType);
    auto cleanUpRegion = builder->makeCodeRegionWithSignature({}, Type::getVoidType());

    auto newBuilder = basicMakeObject<SSABuilder> ();
    newBuilder->setLexicalScope(builder->getLexicalScope());
    newBuilder->setSourcePosition(node->sourcePosition);
    newBuilder->setCodeRegion(bodyRegion);
    newBuilder->makeBasicBlockHere();

    auto newCleanUpBuilder = basicMakeObject<SSABuilder> ();
    newCleanUpBuilder->setLexicalScope(cleanUpBuilder->getLexicalScope());
    newCleanUpBuilder->setSourcePosition(node->sourcePosition);
    newCleanUpBuilder->setCodeRegion(cleanUpRegion);
    newCleanUpBuilder->makeBasicBlockHere();

    auto oldBuilder = builder;
    auto oldCurrentRegion = currentCodeRegion;
    auto oldCleanUpBuilder = cleanUpBuilder;
    auto oldCleanUpRegion = currentCleanUpCodeRegion;

    doWithEnsure([&](){
        builder = newBuilder;
        currentCodeRegion = bodyRegion;
        cleanUpBuilder = newCleanUpBuilder;
        currentCleanUpCodeRegion = cleanUpRegion;

        auto value = visitNodeForValue(node->body);
        if(!builder->isLastTerminator())
            returnValueFromRegion(value);

        if(!cleanUpBuilder->isLastTerminator())
            cleanUpBuilder->returnFromRegion(cleanUpBuilder->literal(getVoidConstant()));
    }, [&](){
        builder = oldBuilder;
        currentCodeRegion = oldCurrentRegion;
        cleanUpBuilder = oldCleanUpBuilder;
        currentCleanUpCodeRegion = oldCleanUpRegion;
    });

    auto result = builder->doWithCleanUp(bodyRegion, cleanUpRegion);
    if(node->analyzedType->isReturnedByReference())
        addFinalizationFor(result, node->analyzedType);
    return result;
}

AnyValuePtr ASTSSACompiler::visitClosureNode(const ASTClosureNodePtr &node)
{
    sysmelAssert(node->analyzedProgramEntity->isCompiledMethod());
    auto method = staticObjectCast<CompiledMethod> (node->analyzedProgramEntity);
    auto functionObject = node->analyzedProgramEntity->asSSAValueRequiredInPosition(builder->getSourcePosition());
    if(node->analyzedType->isClosureType())
    {
        // Fetch the captured values.
        SSAValuePtrList capturedValues;
        auto capturedVariables = method->getCapturedVariables();
        capturedValues.reserve(capturedVariables.size());
        for(auto &variable : capturedVariables)
            capturedValues.push_back(findLocalVariableMapping(variable));

        // Instantiate the closure.
        functionObject = builder->makeClosure(functionObject, capturedValues);
    }

    return functionObject;
}

AnyValuePtr ASTSSACompiler::visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node)
{
    auto oldScope = builder->getLexicalScope();
    auto oldCleanUp = cleanUpBuilder->getLexicalScope();

    builder->setLexicalScope(node->analyzedScope);
    cleanUpBuilder->setLexicalScope(node->analyzedScope);

    auto result = visitNodeForValue(node->body);
    
    builder->setLexicalScope(oldScope);
    cleanUpBuilder->setLexicalScope(oldCleanUp);
    return result;
}

AnyValuePtr ASTSSACompiler::visitLiteralValueNode(const ASTLiteralValueNodePtr &node)
{
    auto constantValue = node->value->asSSAValueRequiredInPosition(builder->getSourcePosition());
    auto valueType = node->analyzedType;
    if(valueType->isReturnedByReference())
    {
        auto resultLocal = builder->localVariable(valueType->tempRef(), valueType);
        assignInitialValueFrom(resultLocal, valueType, constantValue);
        return resultLocal;
    }

    sysmelAssert(constantValue);
    return constantValue;
}

AnyValuePtr ASTSSACompiler::visitMakeAggregateNode(const ASTMakeAggregateNodePtr &node)
{
    SSAValuePtrList elements;
    elements.reserve(node->elements.size());
    for(auto &el : node->elements)
        elements.push_back(visitNodeForValue(el));

    sysmelAssert(node->analyzedType->isAggregateType());
    return makeAggregateWithElements(staticObjectCast<AggregateType> (node->analyzedType), elements);
}

AnyValuePtr ASTSSACompiler::visitMakeTupleNode(const ASTMakeTupleNodePtr &node)
{
    SSAValuePtrList elements;
    elements.reserve(node->elements.size());
    for(auto &el : node->elements)
    {
        auto value = visitNodeForValue(el);
        if(!el->analyzedType->isVoidType())
            elements.push_back(value);
    }

    if(elements.empty())
    {
        sysmelAssert(node->analyzedType->isVoidType());
        return builder->literalWithType(getVoidConstant(), node->analyzedType);
    }

    sysmelAssert(node->analyzedType->isAggregateType());
    return makeAggregateWithElements(staticObjectCast<AggregateType> (node->analyzedType), elements);
}

AnyValuePtr ASTSSACompiler::visitMakeVectorNode(const ASTMakeVectorNodePtr &node)
{
    SSAValuePtrList elements;
    elements.reserve(node->elements.size());
    for(auto &el : node->elements)
    {
        auto value = visitNodeForValue(el);
        if(!el->analyzedType->isVoidType())
            elements.push_back(value);
    }

    return builder->makeVector(node->analyzedType, elements);
}

AnyValuePtr ASTSSACompiler::visitMakeVariantNode(const ASTMakeVariantNodePtr &node)
{
    auto value = visitNodeForValue(node->value);

    sysmelAssert(node->analyzedType->isVariantType());
    auto variantType = staticObjectCast<VariantType> (node->analyzedType);
    auto result = builder->localVariable(variantType->tempRef(), variantType);
    auto layout = variantType->getLayout().staticAs<AggregateTypeVariantLayout> ();

    auto typeSelectorSlotType = layout->getDataTypeIndexType();
    auto typeSelectorIndex = node->typeSelectorIndex;
    auto typeSelectorSlot = builder->getAggregateSlotReference(typeSelectorSlotType->tempRef(), result, builder->literal(wrapValue(uint64_t(0))));
    auto typeSelectorValue = typeSelectorSlotType->instantiatedWithLiteralValue(wrapValue(typeSelectorIndex));
    builder->storeValueIn(builder->literal(typeSelectorValue), typeSelectorSlot);

    const auto &dataSlotType = variantType->elementTypes[size_t(typeSelectorIndex)];
    auto dataSlotIndex = layout->getElementMemorySlotIndex();
    auto dataSlot = builder->getAggregateSlotReference(dataSlotType->tempRef(), result, builder->literal(wrapValue(dataSlotIndex)));
    assignInitialValueFrom(dataSlot, dataSlotType, value, true);

    addFinalizationFor(result, variantType);
    return result;
}

AnyValuePtr ASTSSACompiler::visitMessageSendNode(const ASTMessageSendNodePtr &node)
{
    SSAValuePtr receiverValue;
    if(node->receiver)
        receiverValue = visitNodeForValue(node->receiver);

    SSAValuePtr selectorValue;
    if(node->selector)
        selectorValue = visitNodeForValue(node->selector);

    auto &resultType = node->analyzedType;
    bool isReturningByReference = resultType->isReturnedByReference();
    bool isDirectCallPassingReceiver = node->analyzedBoundMessageIsDirect && node->receiver && !node->receiver->analyzedType->isVoidType();

    SSAValuePtrList arguments;
    arguments.reserve((isReturningByReference ? 1 : 0) + (isDirectCallPassingReceiver ? 1 : 0) + node->arguments.size());

    // Return by reference hidden pointer.
    SSAValuePtr resultByReference;
    if(isReturningByReference)
    {
        resultByReference = builder->localVariable(resultType->tempRef(), resultType);
        arguments.push_back(resultByReference);
    }

    // Direct call this pointer.
    if(isDirectCallPassingReceiver)
        arguments.push_back(receiverValue);
    
    // Explicit arguments.
    for(auto &arg : node->arguments)
        arguments.push_back(visitNodeForValue(arg));

    // Replace the direct message sends by call.
    sysmelAssert(node->receiver || node->analyzedBoundMessageIsDirect);
    SSAValuePtr sendResult;
    if(node->analyzedBoundMessageIsDirect)
    {
        sendResult = builder->call(node->analyzedType, node->analyzedBoundMessage->asSSAValueRequiredInPosition(node->sourcePosition), arguments);
    }
    else
    {
        sendResult = builder->sendMessage(node->analyzedType, node->calledMessageType,
            selectorValue, receiverValue, arguments, 
            node->useVirtualTable, node->virtualTableSlotIndex, node->virtualTableEntrySlotIndex);
    }

    if(isReturningByReference)
    {
        addFinalizationFor(resultByReference, resultType);
        return resultByReference;
    }

    return sendResult;
}

AnyValuePtr ASTSSACompiler::visitSequenceNode(const ASTSequenceNodePtr &node)
{
    SSAValuePtr result;
    for(auto &expression : node->expressions)
    {
        result = visitNodeForValue(expression);
        if(builder->isLastTerminator())
            break;
    }

    if(!result)
        result = builder->literal(getVoidConstant());
    return result;
}

AnyValuePtr ASTSSACompiler::visitVectorSwizzleNode(const ASTVectorSwizzleNodePtr &node)
{
    auto vector = visitNodeForValue(node->vector);
    return builder->vectorSwizzle(node->analyzedType, vector, node->selectedElements);
}

AnyValuePtr ASTSSACompiler::visitQuasiQuotePatternExpansionNode(const ASTQuasiQuotePatternExpansionNodePtr &node)
{
    auto pattern = visitNodeForValue(node->pattern);

    SSAValuePtrList arguments;
    arguments.reserve(node->arguments.size());
    for(auto &arg : node->arguments)
        arguments.push_back(visitNodeForValue(arg));
    
    //TODO: invoke an intrinsic function for expanding the pattern.
    return pattern;
}

AnyValuePtr ASTSSACompiler::visitQuoteNode(const ASTQuoteNodePtr &node)
{
    return builder->literalWithType(node->expression, node->analyzedType);
}

AnyValuePtr ASTSSACompiler::visitLocalVariableNode(const ASTLocalVariableNodePtr &node)
{
    auto variable = staticObjectCast<Variable> (node->analyzedProgramEntity);
    SSAValuePtr initialValue;
    if(node->initialValue)
        initialValue = visitNodeForValue(node->initialValue);
    else
        initialValue = builder->literalWithType(variable->getValueType()->defaultValue(), variable->getValueType());

    auto variableValue = initialValue;
    auto valueType = variable->getValueType();
    if(node->isMutable || valueType->isReturnedByReference())
    {
        auto referenceType = variable->getReferenceType();
        if(!referenceType->isReferenceLikeType() && valueType->isReturnedByReference())
            referenceType = valueType->tempRef();

        sysmelAssert(referenceType->isReferenceLikeType());
        valueType = variable->getValueType();
        variableValue = builder->localVariable(referenceType, valueType);
        assignInitialValueFrom(variableValue, valueType, initialValue);
    }

    if(!validAnyValue(variable->getName())->isHiddenNameSymbol())
        builder->declareLocalVariable(variableValue, variable);
    mapLocalVariableToValue(variable, variableValue);
    return variableValue;
}

AnyValuePtr ASTSSACompiler::visitGlobalVariableNode(const ASTGlobalVariableNodePtr &node)
{
    return node->analyzedProgramEntity->asSSAValueRequiredInPosition(builder->getSourcePosition());
}

AnyValuePtr ASTSSACompiler::visitFieldVariableAccessNode(const ASTFieldVariableAccessNodePtr &node)
{
    auto aggregate = visitNodeForValue(node->aggregate);
    return builder->getAggregateFieldReference(node->analyzedType, aggregate, node->fieldVariable);
}

AnyValuePtr ASTSSACompiler::visitSlotAccessNode(const ASTSlotAccessNodePtr &node)
{
    auto aggregate = visitNodeForValue(node->aggregate);
    if(node->checkTypeSelectorIndex)
        builder->checkExpectedTypeSelectorValue(aggregate, node->typeSelectorSlotIndex, node->typeSelectorSlotReferenceType, node->expectedTypeSelectorValue);

    auto slotIndex = node->slotIndex;
    if(node->isNotPaddedSlotIndex)
    {
        auto aggregateType = node->aggregate->analyzedType->asDecayedType();
        sysmelAssert(aggregateType->isAggregateType());
        slotIndex = aggregateType.staticAs<AggregateType>()->getLayout()->getIndexForNonPaddingSlot(slotIndex);
    }

    return builder->getAggregateSlotReference(node->analyzedType, aggregate, builder->literal(wrapValue(slotIndex)));
}

AnyValuePtr ASTSSACompiler::visitVariableAccessNode(const ASTVariableAccessNodePtr &node)
{
    auto localValue = findLocalVariableMapping(node->variable);
    return localValue ? localValue : node->variable->asSSAValueRequiredInPosition(node->sourcePosition);
}

AnyValuePtr ASTSSACompiler::visitLocalImmutableAccessNode(const ASTLocalImmutableAccessNodePtr &node)
{
    auto result = findLocalVariableMapping(node->bindingName);
    sysmelAssert(result);
    return result;
}

AnyValuePtr ASTSSACompiler::visitProgramEntityNode(const ASTProgramEntityNodePtr &node)
{
    return builder->literal(node->analyzedProgramEntity);
}

AnyValuePtr ASTSSACompiler::visitFunctionalNode(const ASTFunctionalNodePtr &node)
{
    sysmelAssert(node->analyzedProgramEntity->isCompiledMethod());
    auto method = staticObjectCast<CompiledMethod> (node->analyzedProgramEntity);
    auto functionObject = node->analyzedProgramEntity->asSSAValueRequiredInPosition(builder->getSourcePosition());
    if(node->analyzedType->isClosureType())
    {
        // Fetch the captured values.
        SSAValuePtrList capturedValues;
        auto capturedVariables = method->getCapturedVariables();
        capturedValues.reserve(capturedVariables.size());
        for(auto &variable : capturedVariables)
            capturedValues.push_back(findLocalVariableMapping(variable));

        // Instantiate the closure.
        functionObject = builder->makeClosure(functionObject, capturedValues);

        // Bind the closure on the local variable map.
        if(!validAnyValue(method->getName())->isAnonymousNameSymbol())
            mapLocalVariableToValue(method, functionObject);
    }

    return functionObject;
}

AnyValuePtr ASTSSACompiler::visitNamespaceNode(const ASTNamespaceNodePtr &node)
{
    return builder->literal(node->analyzedProgramEntity);
}

AnyValuePtr ASTSSACompiler::visitTypeNode(const ASTTypeNodePtr &node)
{
    return builder->literal(node->analyzedProgramEntity);
}

AnyValuePtr ASTSSACompiler::visitProgramEntityExtensionNode(const ASTProgramEntityExtensionNodePtr &node)
{
    return builder->literal(node->analyzedProgramEntity);
}

AnyValuePtr ASTSSACompiler::visitReinterpretCastNode(const ASTReinterpretCastNodePtr &node)
{
    auto value = visitNodeForValue(node->expression);
    return builder->bitcast(node->analyzedType, value);
}

AnyValuePtr ASTSSACompiler::visitValueAsVoidTypeConversionNode(const ASTValueAsVoidTypeConversionNodePtr &node)
{
    visitNodeForValue(node->expression);
    return builder->literal(getVoidConstant());
}

AnyValuePtr ASTSSACompiler::visitUpcastTypeConversionNode(const ASTUpcastTypeConversionNodePtr &node)
{
    auto value = visitNodeForValue(node->expression);
    if(node->analyzedType->isVoidType())
        return builder->literalWithType(getVoidConstant(), node->analyzedType);
    return builder->upcast(node->analyzedType, value);
}

AnyValuePtr ASTSSACompiler::visitDowncastTypeConversionNode(const ASTDowncastTypeConversionNodePtr &node)
{
    return builder->downcast(node->analyzedType, visitNodeForValue(node->expression));
}

AnyValuePtr ASTSSACompiler::visitValueAsReferenceReinterpretConversionNode(const ASTValueAsReferenceReinterpretConversionNodePtr &node)
{
    sysmelAssert(node->expression->analyzedType->isPassedByReference());
    auto result = visitNodeForValue(node->expression);
    sysmelAssert(result->getValueType()->isReferenceLikeType());
    return result;
}

SSACodeRegionPtr ASTSSACompiler::buildRegionForNode(const ASTNodePtr &node)
{
    if(!node)
        return nullptr;

    auto region = builder->makeCodeRegionWithSignature({}, node->analyzedType);
    buildRegionForNodeWith(region, node, [&](){
        auto value = visitNodeForValue(node);
        if(!builder->isLastTerminator())
            returnValueFromRegion(value);
    });
    return region;
}

AnyValuePtr ASTSSACompiler::visitIfNode(const ASTIfNodePtr &node)
{
    auto condition = visitNodeForValue(node->condition);
    auto trueRegion = buildRegionForNode(node->trueExpression);
    auto falseRegion = buildRegionForNode(node->falseExpression);
    auto result = builder->ifTrueIfFalse(node->analyzedType, condition, trueRegion, falseRegion);
    if(node->trueExpression && node->trueExpression->analyzedType->isControlFlowEscapeType()
        && node->falseExpression && node->falseExpression->analyzedType->isControlFlowEscapeType())
        return builder->unreachableInstruction();

    if(node->analyzedType->isReturnedByReference())
        addFinalizationFor(result, node->analyzedType);

    return result;
}

AnyValuePtr ASTSSACompiler::visitWhileNode(const ASTWhileNodePtr &node)
{
    auto conditionRegion = buildRegionForNode(node->condition);
    auto bodyRegion = buildRegionForNode(node->bodyExpression);
    auto continueRegion = buildRegionForNode(node->continueExpression);
    return builder->whileDoContinueWith(conditionRegion, bodyRegion, continueRegion);
}

AnyValuePtr ASTSSACompiler::visitDoWhileNode(const ASTDoWhileNodePtr &node)
{
    auto bodyRegion = buildRegionForNode(node->bodyExpression);
    auto conditionRegion = buildRegionForNode(node->condition);
    auto continueRegion = buildRegionForNode(node->continueExpression);
    return builder->doWhileContinueWith(bodyRegion, conditionRegion, continueRegion);
}

AnyValuePtr ASTSSACompiler::visitReturnNode(const ASTReturnNodePtr &node)
{
    SSAValuePtr resultValue;
    if(node->expression)
        resultValue = visitNodeForValue(node->expression);
    else
        resultValue = builder->literal(getVoidConstant());

    returnValueFromFunction(resultValue);
    return builder->literal(getVoidConstant());
}

AnyValuePtr ASTSSACompiler::visitContinueNode(const ASTContinueNodePtr &)
{
    return builder->continueInstruction();
}

AnyValuePtr ASTSSACompiler::visitBreakNode(const ASTBreakNodePtr &)
{
    return builder->breakInstruction();
}

AnyValuePtr ASTSSACompiler::visitEvaluatePatternWithValueNode(const ASTEvaluatePatternWithValueNodePtr &node)
{
    auto patternValue = visitNodeForValue(node->valueNode);

    auto patternRegion = buildRegionForNode(node->patternEvaluationNode);
    SSACodeRegionPtr successRegion, failureRegion;
    if(node->successAction)
        successRegion = buildRegionForNode(node->successAction);
    if(node->failureAction)
        failureRegion = buildRegionForNode(node->failureAction);
    
    auto result = builder->evaluatePattern(node->analyzedType, patternRegion, successRegion, failureRegion);
    if(node->successAction && node->successAction->analyzedType->isControlFlowEscapeType()
        && node->failureAction && node->failureAction->analyzedType->isControlFlowEscapeType())
        return builder->unreachableInstruction();

    if(node->analyzedType->isReturnedByReference())
        addFinalizationFor(result, node->analyzedType);
    return result;
}

AnyValuePtr ASTSSACompiler::visitFailPatternNode(const ASTFailPatternNodePtr &)
{
    return builder->failPattern();
}

AnyValuePtr ASTSSACompiler::visitTrapNode(const ASTTrapNodePtr &node)
{
    return builder->trap(node->reason);
}

void ASTSSACompiler::buildRegionForSourcePositionWith(const SSACodeRegionPtr &region, const ASTSourcePositionPtr &sourcePosition, const ASTSSACodeRegionBuildingBlock &aBlock)
{
    auto regionBuilder = basicMakeObject<SSABuilder> ();
    regionBuilder->setSourcePosition(sourcePosition);
    regionBuilder->setCodeRegion(region);
    regionBuilder->makeBasicBlockHere();

    auto oldBuilder = builder;
    auto oldRegion = currentCodeRegion;
    builder = regionBuilder;
    currentCodeRegion = region;

    doWithEnsure(aBlock, [&](){
        builder = oldBuilder;
        currentCodeRegion = oldRegion;
    });
}

void ASTSSACompiler::buildRegionForNodeWith(const SSACodeRegionPtr &region, const ASTNodePtr &node, const ASTSSACodeRegionBuildingBlock &aBlock)
{
    return buildRegionForSourcePositionWith(region, node->sourcePosition, aBlock);
}

void ASTSSACompiler::returnValueFromFunction(const SSAValuePtr &result)
{
    auto sanitizedResult = prepareForReturningValueFromRegion(result, currentSSAFunction->getMainCodeRegion());
    builder->returnFromFunction(sanitizedResult);
}

void ASTSSACompiler::returnValueFromRegion(const SSAValuePtr &result)
{
    auto sanitizedResult = prepareForReturningValueFromRegion(result, currentCodeRegion);
    builder->returnFromRegion(sanitizedResult);
}

SSAValuePtr ASTSSACompiler::prepareForReturningValueFromRegion(const SSAValuePtr &result, const SSACodeRegionPtr &returningRegion)
{
    if(returningRegion->isReturningByReference())
    {
        auto resultArgument = returningRegion->getArgument(0);
        sysmelAssert(!result->getValueType()->isVoidType() && resultArgument->getValueType()->isPointerLikeType());
        assignInitialValueFrom(resultArgument, resultArgument->getValueType()->getBaseType(), result);
        return builder->literal(getVoidConstant());
    }

    return result;
}
} // End of namespace Environment
} // End of namespace Sysmel