#include "Environment/ASTCompileTimeEvaluator.hpp"
#include "Environment/ASTArgumentDefinitionNode.hpp"
#include "Environment/ASTCleanUpScopeNode.hpp"
#include "Environment/ASTClosureNode.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTLexicalScopeNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTMakeAssociationNode.hpp"
#include "Environment/ASTMakeDictionaryNode.hpp"
#include "Environment/ASTMakeLiteralArrayNode.hpp"
#include "Environment/ASTMakeTupleNode.hpp"
#include "Environment/ASTMakeVectorNode.hpp"
#include "Environment/ASTMessageChainMessageNode.hpp"
#include "Environment/ASTMessageChainNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTPragmaNode.hpp"
#include "Environment/ASTQuasiQuoteNode.hpp"
#include "Environment/ASTQuasiUnquoteNode.hpp"
#include "Environment/ASTQuoteNode.hpp"
#include "Environment/ASTParseErrorNode.hpp"
#include "Environment/ASTSequenceNode.hpp"
#include "Environment/ASTSpliceNode.hpp"

#include "Environment/ASTQuasiQuoteExpander.hpp"
#include "Environment/ASTQuasiQuotePatternExpansionNode.hpp"

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
#include "Environment/ASTValueAsReferenceReinterpretConversionNode.hpp"

#include "Environment/ASTIfNode.hpp"
#include "Environment/ASTWhileNode.hpp"
#include "Environment/ASTDoWhileNode.hpp"
#include "Environment/ASTReturnNode.hpp"
#include "Environment/ASTBreakNode.hpp"
#include "Environment/ASTContinueNode.hpp"

#include "Environment/CompileTimeCleanUpScope.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/Variable.hpp"

#include "Environment/ClosureType.hpp"
#include "Environment/PrimitiveVectorType.hpp"
#include "Environment/TupleType.hpp"
#include "Environment/LiteralAssociation.hpp"
#include "Environment/LiteralDictionary.hpp"

#include "Environment/BootstrapMethod.hpp"
#include "Environment/CompiledMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

#include "Environment/ControlFlowUtilities.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/Error.hpp"

namespace Sysmel
{
namespace Environment
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
    sysmelAssert(!currentCleanUpScope);
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
    auto method = staticObjectCast<CompiledMethod> (node->analyzedProgramEntity);
    auto closureType = staticObjectCast<ClosureType> (node->analyzedType);
    return closureType->makeValueWithEnvironmentAndImplementation(currentCleanUpScope, method);
}

AnyValuePtr ASTCompileTimeEvaluator::visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &)
{
    signalNewWithMessage<Error> ("Identifier reference nodes cannot be evaluated.");
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
    auto key = visitNode(node->key);
    auto value = visitNode(node->value);
    return LiteralAssociation::makeWith(key, value);
}

AnyValuePtr ASTCompileTimeEvaluator::visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node)
{
    AnyValuePtrList elements;
    elements.reserve(node->elements.size());
    for(auto &el : node->elements)
        elements.push_back(visitNode(el));
    return LiteralDictionary::makeFor(elements);
}

AnyValuePtr ASTCompileTimeEvaluator::visitMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &node)
{
    AnyValuePtrList result;
    result.reserve(node->elements.size());
    for(auto & el : node->elements)
        result.push_back(visitNode(el));

    return wrapValue(result);
}

AnyValuePtr ASTCompileTimeEvaluator::visitMakeTupleNode(const ASTMakeTupleNodePtr &node)
{
    AnyValuePtrList elements;
    elements.reserve(node->elements.size());

    for(auto &el : node->elements)
    {
        auto elementValue = visitNode(el);
        if(!el->analyzedType->isVoidType())
            elements.push_back(elementValue);
    }

    switch(elements.size())
    {
    case 0:
        return getVoidConstant();
    case 1:
        return elements.back();
    default:
        sysmelAssert(node->analyzedType->isTupleType());
        return node->analyzedType.staticAs<TupleType> ()->makeWithElements(elements);
    }
}

AnyValuePtr ASTCompileTimeEvaluator::visitMakeVectorNode(const ASTMakeVectorNodePtr &node)
{
    auto resultVectorType = node->analyzedType.staticAs<PrimitiveVectorType> ();
    
    AnyValuePtrList elements;
    elements.reserve(resultVectorType->elements);
    for(auto &arg : node->elements)
    {
        auto elementOrVector = visitNode(arg);
        if(elementOrVector->isPrimitiveVectorTypeValue())
        {
            auto &subvector = elementOrVector.staticAs<PrimitiveVectorTypeValue> ()->elements;
            elements.insert(elements.end(), subvector.begin(), subvector.end());
        }
        else
        {
            elements.push_back(elementOrVector);
        }
    }

    return resultVectorType->withValues(elements);
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

AnyValuePtr ASTCompileTimeEvaluator::visitQuasiQuotePatternExpansionNode(const ASTQuasiQuotePatternExpansionNodePtr &node)
{
    auto expander = basicMakeObject<ASTQuasiQuoteExpander> ();
    auto pattern = visitNode(node->pattern);
    sysmelAssert(pattern->isASTNode());

    AnyValuePtrList arguments;
    arguments.reserve(node->arguments.size());
    for(auto &arg : node->arguments)
        arguments.push_back(visitNode(arg));

    return expander->expandPatternWithArguments(staticObjectCast<ASTNode> (pattern), arguments);
}

AnyValuePtr ASTCompileTimeEvaluator::visitLocalVariableNode(const ASTLocalVariableNodePtr &node)
{
    sysmelAssert(currentCleanUpScope);

    AnyValuePtr initialValue;

    auto variable = staticObjectCast<Variable> (node->analyzedProgramEntity);
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
        sysmelAssert(referenceType->isReferenceLikeType());

        auto mutableValue = storeValue->asMutableStoreValue();
        storeValue = referenceType.staticAs<PointerLikeType> ()->makeWithValue(mutableValue);
    }

    currentCleanUpScope->setStoreBinding(node->analyzedProgramEntity, storeValue);

    return storeValue;
}

AnyValuePtr ASTCompileTimeEvaluator::visitGlobalVariableNode(const ASTGlobalVariableNodePtr &node)
{
    return staticObjectCast<Variable> (node->analyzedProgramEntity)->findStoreBindingInCompileTime(currentCleanUpScope)
        ->accessVariableAsReferenceWithType(node->analyzedType);
}

AnyValuePtr ASTCompileTimeEvaluator::visitFieldVariableAccessNode(const ASTFieldVariableAccessNodePtr &node)
{
    auto aggregate = visitNode(node->aggregate);
    return aggregate->getReferenceToFieldWithType(node->fieldVariable, node->analyzedType);
}

AnyValuePtr ASTCompileTimeEvaluator::visitVariableAccessNode(const ASTVariableAccessNodePtr &node)
{
    auto storeBinding = node->variable->findStoreBindingInCompileTime(currentCleanUpScope);
    sysmelAssert(storeBinding);
    
    return storeBinding->accessVariableAsReferenceWithType(node->analyzedType);
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
    auto method = staticObjectCast<CompiledMethod> (node->analyzedProgramEntity);
    if(node->analyzedType->isClosureType())
    {
        auto closureType = staticObjectCast<ClosureType> (node->analyzedType);
        auto closure = closureType->makeValueWithEnvironmentAndImplementation(currentCleanUpScope, method);
        if(!validAnyValue(method->getName())->isAnonymousNameSymbol())
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

AnyValuePtr ASTCompileTimeEvaluator::visitValueAsReferenceReinterpretConversionNode(const ASTValueAsReferenceReinterpretConversionNodePtr &node)
{
    auto value = visitNode(node->expression);
    auto resultType = node->analyzedType;
    sysmelAssert(resultType->isPointerLikeType());
    return resultType.staticAs<PointerLikeType> ()->makeWithValue(value);
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
    sysmelAssert(currentReturnToken);
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
} // End of namespace Environment
} // End of namespace Sysmel