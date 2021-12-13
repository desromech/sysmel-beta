#include "Environment/ASTSSACompiler.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

#include "Environment/ASTNode.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTCleanUpScopeNode.hpp"
#include "Environment/ASTClosureNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTLexicalScopeNode.hpp"
#include "Environment/ASTMakeTupleNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTQuoteNode.hpp"
#include "Environment/ASTSequenceNode.hpp"

#include "Environment/ASTGlobalVariableNode.hpp"
#include "Environment/ASTLocalVariableNode.hpp"
#include "Environment/ASTFieldVariableAccessNode.hpp"
#include "Environment/ASTVariableAccessNode.hpp"
#include "Environment/ASTLocalImmutableAccessNode.hpp"

#include "Environment/ASTFunctionalNode.hpp"
#include "Environment/ASTNamespaceNode.hpp"
#include "Environment/ASTTypeNode.hpp"
#include "Environment/ASTProgramEntityExtensionNode.hpp"

#include "Environment/ASTValueAsVoidTypeConversionNode.hpp"
#include "Environment/ASTUpcastTypeConversionNode.hpp"
#include "Environment/ASTDowncastTypeConversionNode.hpp"

#include "Environment/ASTIfNode.hpp"
#include "Environment/ASTWhileNode.hpp"
#include "Environment/ASTDoWhileNode.hpp"
#include "Environment/ASTReturnNode.hpp"

#include "Environment/CompiledMethod.hpp"
#include "Environment/SSAFunction.hpp"
#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSACodeRegionArgument.hpp"
#include "Environment/SSABasicBlock.hpp"
#include "Environment/SSABuilder.hpp"

#include "Environment/SSAConstantLiteralValue.hpp"

#include "Environment/SSABreakInstruction.hpp"
#include "Environment/SSACallInstruction.hpp"
#include "Environment/SSAContinueInstruction.hpp"
#include "Environment/SSADoWithCleanupInstruction.hpp"
#include "Environment/SSADoWhileInstruction.hpp"
#include "Environment/SSAGetAggregateFieldReferenceInstruction.hpp"
#include "Environment/SSAIfInstruction.hpp"
#include "Environment/SSALoadInstruction.hpp"
#include "Environment/SSALocalVariableInstruction.hpp"
#include "Environment/SSAMakeAggregateInstruction.hpp"
#include "Environment/SSAMakeClosureInstruction.hpp"
#include "Environment/SSAReturnFromFunctionInstruction.hpp"
#include "Environment/SSASendMessageInstruction.hpp"
#include "Environment/SSAStoreInstruction.hpp"
#include "Environment/SSAUnreachableInstruction.hpp"
#include "Environment/SSAWhileInstruction.hpp"

#include "Environment/SSADowncastInstruction.hpp"
#include "Environment/SSAUpcastInstruction.hpp"

#include "Environment/FunctionalType.hpp"
#include "Environment/ArgumentVariable.hpp"
#include "Environment/LocalVariable.hpp"

#include "Environment/ControlFlowUtilities.hpp"

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

    assert(value->isSSAValue());
    return staticObjectCast<SSAValue> (value);
}

void ASTSSACompiler::compileMethodBody(const CompiledMethodPtr &method, const SSAFunctionPtr &ssaFunction, const ASTNodePtr &node)
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
    builder->setCodeRegion(currentCodeRegion);
    builder->makeBasicBlockHere();

    cleanUpBuilder = basicMakeObject<SSABuilder> ();
    cleanUpBuilder->setSourcePosition(node->sourcePosition);
    cleanUpBuilder->setCodeRegion(currentCleanUpCodeRegion);
    cleanUpBuilder->makeBasicBlockHere();

    size_t argumentsOffset = 0;
    auto functionalType = method->getFunctionalType();
    auto receiverType = functionalType->getReceiverType();
    if(!receiverType->isVoidType())
    {
        argumentsOffset = 1;
    }

    const auto &methodArguments = method->getArguments();
    const auto &methodCodeRegionArguments = methodCodeRegion->getArguments();
    for(size_t i = 0; i < methodArguments.size(); ++i)
        localVariableMap.insert({methodArguments[i], methodCodeRegionArguments[argumentsOffset + i]});
    
    auto resultValue = visitNodeForValue(node);
    if(!builder->isLastTerminator())
        builder->returnFromFunction(resultValue);

    if(!cleanUpBuilder->isLastTerminator())
        cleanUpBuilder->returnFromRegion(cleanUpBuilder->literal(getVoidConstant()));

    builder->setCodeRegion(methodCodeRegion);
    builder->makeBasicBlockHere();
    builder->doWithCleanUp(mainCodeRegion, mainCleanUpCodeRegion);
    builder->unreachableInstruction();
}

AnyValuePtr ASTSSACompiler::visitCallNode(const ASTCallNodePtr &node)
{
    auto functionValue = visitNodeForValue(node->function);
    SSAValuePtrList arguments;
    arguments.reserve(node->arguments.size());
    for(auto &arg : node->arguments)
        arguments.push_back(visitNodeForValue(arg));

    return builder->call(node->analyzedType, functionValue, arguments);
}

AnyValuePtr ASTSSACompiler::visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node)
{
    auto bodyRegion = builder->makeCodeRegionWithSignature({}, node->analyzedType);
    auto cleanUpRegion = builder->makeCodeRegionWithSignature({}, Type::getVoidType());

    auto newBuilder = basicMakeObject<SSABuilder> ();
    newBuilder->setSourcePosition(node->sourcePosition);
    newBuilder->setCodeRegion(bodyRegion);
    newBuilder->makeBasicBlockHere();

    auto newCleanUpBuilder = basicMakeObject<SSABuilder> ();
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
            builder->returnFromRegion(value);

        if(!cleanUpBuilder->isLastTerminator())
            cleanUpBuilder->returnFromRegion(cleanUpBuilder->literal(getVoidConstant()));
    }, [&](){
        builder = oldBuilder;
        currentCodeRegion = oldCurrentRegion;
        cleanUpBuilder = oldCleanUpBuilder;
        currentCleanUpCodeRegion = oldCleanUpRegion;
    });

    return builder->doWithCleanUp(bodyRegion, cleanUpRegion);
}

AnyValuePtr ASTSSACompiler::visitClosureNode(const ASTClosureNodePtr &node)
{
    assert(node->analyzedProgramEntity->isCompiledMethod());
    auto method = staticObjectCast<CompiledMethod> (node->analyzedProgramEntity);
    auto functionObject = node->analyzedProgramEntity->asSSAValueRequiredInPosition(builder->getSourcePosition());
    if(node->analyzedType->isClosureType())
    {
        // Fetch the captured values.
        SSAValuePtrList capturedValues;
        auto capturedVariables = method->getCapturedVariables();
        capturedValues.reserve(capturedVariables.size());
        for(auto &variable : capturedVariables)
            capturedValues.push_back(localVariableMap.find(variable)->second);

        // Instantiate the closure.
        functionObject = builder->makeClosure(functionObject, capturedValues);
    }

    return functionObject;
}

AnyValuePtr ASTSSACompiler::visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node)
{
    return visitNodeForValue(node->body);
}

AnyValuePtr ASTSSACompiler::visitLiteralValueNode(const ASTLiteralValueNodePtr &node)
{
    return node->value->asSSAValueRequiredInPosition(builder->getSourcePosition());
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
        assert(node->analyzedType->isVoidType());
        return builder->literalWithType(getVoidConstant(), node->analyzedType);
    }

    return builder->makeAggregate(node->analyzedType, elements);
}

AnyValuePtr ASTSSACompiler::visitMessageSendNode(const ASTMessageSendNodePtr &node)
{
    SSAValuePtr receiverValue;
    if(node->receiver)
        receiverValue = visitNodeForValue(node->receiver);

    SSAValuePtr selectorValue;
    if(node->selector)
        selectorValue = visitNodeForValue(node->selector);

    SSAValuePtrList arguments;
    arguments.reserve(node->arguments.size());
    for(auto &arg : node->arguments)
        arguments.push_back(visitNodeForValue(arg));

    // Replace the direct message sends by call.
    assert(node->receiver || node->analyzedBoundMessageIsDirect);
    if(node->analyzedBoundMessageIsDirect)
    {
        if(node->receiver && !node->receiver->analyzedType->isVoidType())
            arguments.insert(arguments.begin(), receiverValue);

        return builder->call(node->analyzedType, node->analyzedBoundMessage->asSSAValueRequiredInPosition(node->sourcePosition), arguments);
    }

    return builder->sendMessage(node->analyzedType, selectorValue, receiverValue, arguments);
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
    if(node->isMutable)
    {
        auto referenceType = variable->getReferenceType();
        assert(referenceType->isReferenceLikeType());
        variableValue = builder->localVariable(referenceType, variable->getValueType());
        builder->storeValueIn(initialValue, variableValue);
    }

    localVariableMap.insert({variable, variableValue});
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

AnyValuePtr ASTSSACompiler::visitVariableAccessNode(const ASTVariableAccessNodePtr &node)
{
    auto it = localVariableMap.find(node->variable);
    auto variableValue = it != localVariableMap.end() ? it->second : node->variable->asSSAValueRequiredInPosition(node->sourcePosition);
    return variableValue;
}

AnyValuePtr ASTSSACompiler::visitLocalImmutableAccessNode(const ASTLocalImmutableAccessNodePtr &node)
{
    return localVariableMap.find(node->bindingName)->second;
}

AnyValuePtr ASTSSACompiler::visitProgramEntityNode(const ASTProgramEntityNodePtr &node)
{
    return builder->literal(node->analyzedProgramEntity);
}

AnyValuePtr ASTSSACompiler::visitFunctionalNode(const ASTFunctionalNodePtr &node)
{
    assert(node->analyzedProgramEntity->isCompiledMethod());
    auto method = staticObjectCast<CompiledMethod> (node->analyzedProgramEntity);
    auto functionObject = node->analyzedProgramEntity->asSSAValueRequiredInPosition(builder->getSourcePosition());
    if(node->analyzedType->isClosureType())
    {
        // Fetch the captured values.
        SSAValuePtrList capturedValues;
        auto capturedVariables = method->getCapturedVariables();
        capturedValues.reserve(capturedVariables.size());
        for(auto &variable : capturedVariables)
            capturedValues.push_back(localVariableMap.find(variable)->second);

        // Instantiate the closure.
        functionObject = builder->makeClosure(functionObject, capturedValues);

        // Bind the closure on the local variable map.
        if(!validAnyValue(method->getName())->isAnonymousNameSymbol())
            localVariableMap.insert({method, functionObject});
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

SSACodeRegionPtr ASTSSACompiler::buildRegionForNode(const ASTNodePtr &node)
{
    if(!node)
        return nullptr;

    auto region = builder->makeCodeRegionWithSignature({}, node->analyzedType);
    buildRegionForNodeWith(region, node, [&](){
        auto value = visitNodeForValue(node);
        if(!builder->isLastTerminator())
            builder->returnFromRegion(value);
    });
    return region;
}

AnyValuePtr ASTSSACompiler::visitIfNode(const ASTIfNodePtr &node)
{
    auto condition = visitNodeForValue(node->condition);
    auto trueRegion = buildRegionForNode(node->trueExpression);
    auto falseRegion = buildRegionForNode(node->falseExpression);
    auto result = builder->ifTrueIfFalse(node->analyzedType, condition, trueRegion, falseRegion);
    if(node->trueExpression->analyzedType->isControlFlowEscapeType() && node->falseExpression->analyzedType->isControlFlowEscapeType())
        return builder->unreachableInstruction();

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
    return builder->returnFromFunction(resultValue);
}

AnyValuePtr ASTSSACompiler::visitContinueNode(const ASTContinueNodePtr &)
{
    return builder->continueInstruction();
}

AnyValuePtr ASTSSACompiler::visitBreakNode(const ASTBreakNodePtr &)
{
    return builder->breakInstruction();
}

void ASTSSACompiler::buildRegionForNodeWith(const SSACodeRegionPtr &region, const ASTNodePtr &node, const ASTSSACodeRegionBuildingBlock &aBlock)
{
    auto regionBuilder = basicMakeObject<SSABuilder> ();
    regionBuilder->setSourcePosition(node->sourcePosition);
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

} // End of namespace Environment
} // End of namespace Sysmel