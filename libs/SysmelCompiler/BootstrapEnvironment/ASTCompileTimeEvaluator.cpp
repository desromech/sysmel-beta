#include "sysmel/BootstrapEnvironment/ASTCompileTimeEvaluator.hpp"
#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCleanUpScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTClosureNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCallNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLexicalScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeAssociationNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeDictionaryNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeLiteralArrayNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeTupleNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainMessageNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTPragmaNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuasiQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuasiUnquoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTParseErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSequenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSpliceNode.hpp"

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
#include "sysmel/BootstrapEnvironment/ASTBreakNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTContinueNode.hpp"

#include "sysmel/BootstrapEnvironment/CompileTimeCleanUpScope.hpp"
#include "sysmel/BootstrapEnvironment/PointerLikeType.hpp"
#include "sysmel/BootstrapEnvironment/Variable.hpp"

#include "sysmel/BootstrapEnvironment/ClosureType.hpp"

#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/CompiledMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

#include "sysmel/BootstrapEnvironment/ControlFlowUtilities.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTCompileTimeEvaluator> ASTCompileTimeEvaluatorTypeRegistration;

struct ASTCompileTimeEvaluatorBreakException
{
};

struct ASTCompileTimeEvaluatorContinueException
{
};

struct ASTCompileTimeEvaluatorReturnException
{
    void *targetToken;
    AnyValuePtr value;
};

AnyValuePtr ASTCompileTimeEvaluator::visitNodeInNewCleanUpScope(const ASTNodePtr &node)
{
    auto oldCleanUpScope = currentCleanUpScope;
    currentCleanUpScope = CompileTimeCleanUpScope::makeWithParent(currentCleanUpScope);

    return doWithEnsure([&](){
        return visitNode(node);
    }, [&](){
        currentCleanUpScope = oldCleanUpScope;
    });
}

AnyValuePtr ASTCompileTimeEvaluator::visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node)
{
    return visitNodeInNewCleanUpScope(node->body);
}

AnyValuePtr ASTCompileTimeEvaluator::evaluateMethodBodyNode(const CompileTimeCleanUpScopePtr &initialEnvironment, const ASTNodePtr &node)
{
    assert(!currentCleanUpScope);
    currentCleanUpScope = initialEnvironment;

    return visitNodeCachingExplicitReturns(node);
}

AnyValuePtr ASTCompileTimeEvaluator::visitNodeCachingExplicitReturns(const ASTNodePtr &node)
{
    int returnToken = 0;
    auto oldReturnToken = currentReturnToken;
    currentReturnToken = &returnToken;
    AnyValuePtr result;

    try
    {
        result = visitNode(node);
        currentReturnToken = oldReturnToken;
    }
    catch(ASTCompileTimeEvaluatorReturnException &e)
    {
        if(e.targetToken != &returnToken)
            throw e;
        
        currentReturnToken = &returnToken;
        result = e.value;
    }

    return result;

}
AnyValuePtr ASTCompileTimeEvaluator::visitClosureNode(const ASTClosureNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitCallNode(const ASTCallNodePtr &node)
{
    auto calledFunction = visitNode(node->function);
    AnyValuePtrList arguments;
    arguments.reserve(node->arguments.size());
    for(auto &arg : node->arguments)
        arguments.push_back(visitNode(arg));
    return calledFunction->applyWithArguments(arguments);
}

AnyValuePtr ASTCompileTimeEvaluator::visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node)
{
    return visitNode(node->body);
}

AnyValuePtr ASTCompileTimeEvaluator::visitLiteralValueNode(const ASTLiteralValueNodePtr &node)
{
    return node->value;
}

AnyValuePtr ASTCompileTimeEvaluator::visitMakeAssociationNode(const ASTMakeAssociationNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitMakeTupleNode(const ASTMakeTupleNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitMessageSendNode(const ASTMessageSendNodePtr &node)
{
    AnyValuePtr receiver = node->receiver ? visitNode(node->receiver) : nullptr;
    AnyValuePtr selector = visitNode(node->selector);

    AnyValuePtrList arguments;
    arguments.reserve(node->arguments.size());
    for(const auto &arg : node->arguments)
        arguments.push_back(visitNode(arg));

    if(node->analyzedBoundMessage && node->analyzedBoundMessageIsDirect)
        return node->analyzedBoundMessage->runWithArgumentsIn(selector, arguments, receiver);
    
    return validAnyValue(receiver)->performWithArguments(selector, arguments);
}

AnyValuePtr ASTCompileTimeEvaluator::visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTCompileTimeEvaluator::visitQuoteNode(const ASTQuoteNodePtr &node)
{
    return node->expression;
}

AnyValuePtr ASTCompileTimeEvaluator::visitSequenceNode(const ASTSequenceNodePtr &node)
{
    auto result = getVoidConstant();
    for(const auto &expression : node->expressions)
        result = visitNode(expression);
    return result;
}

AnyValuePtr ASTCompileTimeEvaluator::visitLocalVariableNode(const ASTLocalVariableNodePtr &node)
{
    assert(currentCleanUpScope);

    AnyValuePtr initialValue;

    auto variable = std::static_pointer_cast<Variable> (node->analyzedProgramEntity);
    if(node->initialValue)
    {
        initialValue = visitNode(node->initialValue);
    }
    else
    {
        initialValue = variable->getValueType()->defaultValue();
    }

    // If the variable is mutable, we have to wrap the value in a box.
    auto storeValue = validAnyValue(initialValue);
    if(node->isMutable)
    {
        auto referenceType = variable->getReferenceType();
        assert(referenceType->isReferenceLikeType());

        auto mutableValue = storeValue->asMutableStoreValue();
        storeValue = std::static_pointer_cast<PointerLikeType> (referenceType)->makeWithValue(mutableValue);
    }

    currentCleanUpScope->setStoreBinding(node->analyzedProgramEntity, storeValue);

    return storeValue;
}

AnyValuePtr ASTCompileTimeEvaluator::visitGlobalVariableNode(const ASTGlobalVariableNodePtr &node)
{
    return std::static_pointer_cast<Variable> (node->analyzedProgramEntity)->findStoreBindingInCompileTime(currentCleanUpScope)
        ->accessVariableAsReferenceWithType(node->analyzedType);
}

AnyValuePtr ASTCompileTimeEvaluator::visitVariableAccessNode(const ASTVariableAccessNodePtr &node)
{
    auto storeBinding = node->variable->findStoreBindingInCompileTime(currentCleanUpScope);
    assert(storeBinding);
    
    return node->isAccessedByReference ?
        storeBinding->accessVariableAsReferenceWithType(node->analyzedType)
        : storeBinding->accessVariableAsValueWithType(node->analyzedType);
}


AnyValuePtr ASTCompileTimeEvaluator::visitLocalImmutableAccessNode(const ASTLocalImmutableAccessNodePtr &node)
{
    return currentCleanUpScope->lookupStoreBindingRecursively(node->bindingName);
}

AnyValuePtr ASTCompileTimeEvaluator::visitProgramEntityNode(const ASTProgramEntityNodePtr &node)
{
    return node->analyzedProgramEntity;
}

AnyValuePtr ASTCompileTimeEvaluator::visitFunctionalNode(const ASTFunctionalNodePtr &node)
{
    auto method = std::static_pointer_cast<CompiledMethod> (node->analyzedProgramEntity);
    if(node->analyzedType->isClosureType())
    {
        auto closureType = std::static_pointer_cast<ClosureType> (node->analyzedType);
        auto closure = closureType->makeValueWithEnvironmentAndImplementation(currentCleanUpScope, method);
        currentCleanUpScope->setStoreBinding(method, closure);
        return closure;
    }

    return method->asFunctionalValue();
}

AnyValuePtr ASTCompileTimeEvaluator::visitNamespaceNode(const ASTNamespaceNodePtr &node)
{
    if(node->body)
        visitNodeInNewCleanUpScope(node->body);

    return node->analyzedProgramEntity;
}

AnyValuePtr ASTCompileTimeEvaluator::visitTypeNode(const ASTTypeNodePtr &node)
{
    return node->analyzedProgramEntity;
}

AnyValuePtr ASTCompileTimeEvaluator::visitProgramEntityExtensionNode(const ASTProgramEntityExtensionNodePtr &node)
{
    return node->analyzedProgramEntity;
}

AnyValuePtr ASTCompileTimeEvaluator::visitValueAsVoidTypeConversionNode(const ASTValueAsVoidTypeConversionNodePtr &node)
{
    visitNode(node->expression);
    return getVoidConstant();
}

AnyValuePtr ASTCompileTimeEvaluator::visitUpcastTypeConversionNode(const ASTUpcastTypeConversionNodePtr &node)
{
    return visitNode(node->expression);
}

AnyValuePtr ASTCompileTimeEvaluator::visitDowncastTypeConversionNode(const ASTDowncastTypeConversionNodePtr &node)
{
    auto value = visitNode(node->expression);
    auto valueType = validAnyValue(value)->getType();
    auto expectedType = node->analyzedType;
    if(!valueType->isSubtypeOf(expectedType))
        signalNewWithMessage<Error> (formatString("Cannot peform down-cast of value of type {0} into value of type {1}.", {valueType->printString(), expectedType->printString()}));

    return value;
}

AnyValuePtr ASTCompileTimeEvaluator::visitIfNode(const ASTIfNodePtr &node)
{
    auto result = getVoidConstant();

    if(visitNode(node->condition)->unwrapAsBoolean())
    {
        if(node->trueExpression)
            result = visitNode(node->trueExpression);
    }
    else
    {
        if(node->falseExpression)
            result = visitNode(node->falseExpression);
    }

    if(node->analyzedType->isVoidType())
        result = getVoidConstant();
    return result;
}

AnyValuePtr ASTCompileTimeEvaluator::visitWhileNode(const ASTWhileNodePtr &node)
{
    while(node->condition ? visitNode(node->condition)->unwrapAsBoolean() : true)
    {
        if(node->bodyExpression)
        {
            try
            {
                visitNode(node->bodyExpression);
            }
            catch(ASTCompileTimeEvaluatorBreakException &)
            {
                return getVoidConstant();
            }
            catch(ASTCompileTimeEvaluatorContinueException &)
            {
                // Nothing is required here
            }
        }

        if(node->continueExpression)
            visitNode(node->continueExpression);
    }

    return getVoidConstant();
}

AnyValuePtr ASTCompileTimeEvaluator::visitDoWhileNode(const ASTDoWhileNodePtr &node)
{
    bool shouldContinue = false;
    do
    {
        if(node->bodyExpression)
        {
            try
            {
                visitNode(node->bodyExpression);
            }
            catch(ASTCompileTimeEvaluatorBreakException &)
            {
                return getVoidConstant();
            }
            catch(ASTCompileTimeEvaluatorContinueException &)
            {
                // Nothing is required here
            }
        }
        
        shouldContinue = node->condition ? visitNode(node->condition)->unwrapAsBoolean() : true;
        if(shouldContinue && node->continueExpression)
            visitNode(node->continueExpression);
    } while(shouldContinue);

    return getVoidConstant();
}

AnyValuePtr ASTCompileTimeEvaluator::visitReturnNode(const ASTReturnNodePtr &node)
{
    assert(currentReturnToken);
    auto result = getVoidConstant();
    if(node->expression)
        result = visitNode(node->expression);
    throw ASTCompileTimeEvaluatorReturnException{currentReturnToken, result};
}

AnyValuePtr ASTCompileTimeEvaluator::visitContinueNode(const ASTContinueNodePtr &)
{
    throw ASTCompileTimeEvaluatorContinueException{};
}

AnyValuePtr ASTCompileTimeEvaluator::visitBreakNode(const ASTBreakNodePtr &)
{
    throw ASTCompileTimeEvaluatorBreakException{};
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius