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

#include "sysmel/BootstrapEnvironment/ASTLocalVariableNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVariableAccessNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLocalImmutableAccessNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTFunctionalNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTNamespaceNode.hpp"

#include "sysmel/BootstrapEnvironment/CompileTimeCleanUpScope.hpp"
#include "sysmel/BootstrapEnvironment/ValueBox.hpp"
#include "sysmel/BootstrapEnvironment/Variable.hpp"

#include "sysmel/BootstrapEnvironment/ClosureType.hpp"

#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/CompiledMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

#include "sysmel/BootstrapEnvironment/ControlFlowUtilities.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTCompileTimeEvaluator> ASTCompileTimeEvaluatorTypeRegistration;

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

AnyValuePtr ASTCompileTimeEvaluator::evaluateMethodBodyNode(const ASTNodePtr &node)
{
    assert(!currentCleanUpScope);
    currentCleanUpScope = CompileTimeCleanUpScope::makeEmpty();

    return visitNode(node);
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

    if(node->initialValue)
    {
        initialValue = visitNode(node->initialValue);
    }
    else
    {
        initialValue = getNilConstant();
        // TODO: Ask the type for the proper default value here.
    }

    // If the variable is mutable, we have to wrap the value in a box.
    auto storeValue = validAnyValue(initialValue);
    if(node->isMutable)
    {
        auto box = std::make_shared<ValueBox> ();
        box->value = initialValue;
        box->type = std::static_pointer_cast<Variable> (node->analyzedProgramEntity)->getValueType();
        storeValue = box;
    }

    currentCleanUpScope->setStoreBinding(node->analyzedProgramEntity, storeValue);

    return storeValue;
}

AnyValuePtr ASTCompileTimeEvaluator::visitVariableAccessNode(const ASTVariableAccessNodePtr &node)
{
    auto storeBinding = currentCleanUpScope->lookupStoreBindingRecursively(node->variable);
    assert(storeBinding);
    
    return node->isAccessedByReference ?
        storeBinding->accessVariableAsReferenceWithType(node->analyzedType)
        : storeBinding->accessVariableAsValueWithType(node->analyzedType);
}


AnyValuePtr ASTCompileTimeEvaluator::visitLocalImmutableAccessNode(const ASTLocalImmutableAccessNodePtr &node)
{
    return currentCleanUpScope->lookupStoreBindingRecursively(node->bindingName);
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius