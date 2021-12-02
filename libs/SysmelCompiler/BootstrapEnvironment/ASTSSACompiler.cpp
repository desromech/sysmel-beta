#include "sysmel/BootstrapEnvironment/ASTSSACompiler.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCallNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCleanUpScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTClosureNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLexicalScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeTupleNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSequenceNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTGlobalVariableNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLocalVariableNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVariableAccessNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLocalImmutableAccessNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTFunctionalNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTNamespaceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTTypeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTProgramEntityExtensionNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTValueAsVoidTypeConversionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTUpcastTypeConversionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTDowncastTypeConversionNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTIfNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTWhileNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTDoWhileNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTReturnNode.hpp"

#include "sysmel/BootstrapEnvironment/CompiledMethod.hpp"
#include "sysmel/BootstrapEnvironment/SSAFunction.hpp"
#include "sysmel/BootstrapEnvironment/SSACodeRegion.hpp"
#include "sysmel/BootstrapEnvironment/SSACodeRegionArgument.hpp"
#include "sysmel/BootstrapEnvironment/SSABasicBlock.hpp"
#include "sysmel/BootstrapEnvironment/SSABuilder.hpp"

#include "sysmel/BootstrapEnvironment/SSAConstantLiteralValue.hpp"

#include "sysmel/BootstrapEnvironment/SSABreakInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSACallInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAContinueInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSADoWithCleanupInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSADoWhileInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAIfInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSALoadInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSALocalVariableInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAMakeAggregateInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAReturnFromFunctionInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSASendMessageInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAStoreInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAUnreachableInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAWhileInstruction.hpp"

#include "sysmel/BootstrapEnvironment/SSADowncastInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAUpcastInstruction.hpp"

#include "sysmel/BootstrapEnvironment/FunctionalType.hpp"
#include "sysmel/BootstrapEnvironment/ArgumentVariable.hpp"
#include "sysmel/BootstrapEnvironment/LocalVariable.hpp"

#include "sysmel/BootstrapEnvironment/ControlFlowUtilities.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
    auto functionObject = node->analyzedProgramEntity->asSSAValueRequiredInPosition(builder->getSourcePosition());
    if(node->analyzedType->isClosureType())
    {
        // TODO: instantiate the closure by passing the captures.
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
    auto method = staticObjectCast<CompiledMethod> (node->analyzedProgramEntity);
    auto functionObject = node->analyzedProgramEntity->asSSAValueRequiredInPosition(builder->getSourcePosition());
    if(node->analyzedType->isClosureType())
    {
        // TODO: instantiate the closure by passing the captures.
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
    return builder->whileDoContinueWith(conditionRegion, bodyRegion, conditionRegion);
}

AnyValuePtr ASTSSACompiler::visitDoWhileNode(const ASTDoWhileNodePtr &node)
{
    auto bodyRegion = buildRegionForNode(node->bodyExpression);
    auto conditionRegion = buildRegionForNode(node->condition);
    auto continueRegion = buildRegionForNode(node->continueExpression);
    return builder->doWhileContinueWith(bodyRegion, conditionRegion, conditionRegion);
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius