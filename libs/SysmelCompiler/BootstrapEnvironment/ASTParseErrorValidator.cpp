#include "sysmel/BootstrapEnvironment/ASTParseErrorValidator.hpp"
#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCleanUpScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTClosureNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTIntrinsicOperationNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLexicalScopeNode.hpp"
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
#include "sysmel/BootstrapEnvironment/CompilationErrors.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTParseErrorValidator> ASTParseErrorValidatorTypeRegistration;

AnyValuePtr ASTParseErrorValidator::visitArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &node)
{
    if(node->identifier)
        visitNode(node->identifier);
    if(node->type)
        visitNode(node->type);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node)
{
    visitNode(node->body);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitClosureNode(const ASTClosureNodePtr &node)
{
    for(const auto &argument : node->arguments)
        visitNode(argument);
    if(node->returnType)
        visitNode(node->returnType);
    visitNode(node->body);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &)
{
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitIntrinsicOperationNode(const ASTIntrinsicOperationNodePtr &node)
{
    for(const auto &param : node->parameters)
        visitNode(param);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node)
{
    visitNode(node->body);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitLiteralValueNode(const ASTLiteralValueNodePtr &)
{
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitMakeAssociationNode(const ASTMakeAssociationNodePtr &node)
{
    visitNode(node->key);
    if(node->value)
        visitNode(node->value);

    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node)
{
    for(const auto &element : node->elements)
        visitNode(element);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &node)
{
    for(const auto &element : node->elements)
        visitNode(element);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitMakeTupleNode(const ASTMakeTupleNodePtr &node)
{
    for(const auto &element : node->elements)
        visitNode(element);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitMessageChainNode(const ASTMessageChainNodePtr &node)
{
    if(node->receiver)
        visitNode(node->receiver);
    for(const auto &message : node->messages)
        visitNode(message);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitMessageChainMessageNode(const ASTMessageChainMessageNodePtr &node)
{
    visitNode(node->selector);
    for(const auto &argument : node->arguments)
        visitNode(argument);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitMessageSendNode(const ASTMessageSendNodePtr &node)
{
    visitNode(node->selector);
    if(node->receiver)
        visitNode(node->receiver);
    for(const auto &argument : node->arguments)
        visitNode(argument);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitParseErrorNode(const ASTParseErrorNodePtr &node)
{
    parseErrorNodes.push_back(node);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitPragmaNode(const ASTPragmaNodePtr &node)
{
    visitNode(node->selector);
    for(const auto &argument : node->arguments)
        visitNode(argument);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &node)
{
    visitNode(node->expression);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitQuasiUnquoteNode(const ASTQuasiUnquoteNodePtr &node)
{
    visitNode(node->expression);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitQuoteNode(const ASTQuoteNodePtr &node)
{
    visitNode(node->expression);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitSequenceNode(const ASTSequenceNodePtr &node)
{
    for(const auto &pragma : node->pragmas)
        visitNode(pragma);
    for(const auto &expr : node->expressions)
        visitNode(expr);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitSpliceNode(const ASTSpliceNodePtr &node)
{
    visitNode(node->expression);
    return AnyValuePtr();
}

CompilationErrorPtr ASTParseErrorValidator::makeCompilationError()
{
    if(parseErrorNodes.empty())
        return nullptr;

    if(parseErrorNodes.size() == 1)
        return parseErrorNodes.back()->asParseError();

    auto errors = std::make_shared<CompilationErrors> ();
    for(const auto &node : parseErrorNodes)
        errors->errors.push_back(node->asParseError());

    return errors;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius