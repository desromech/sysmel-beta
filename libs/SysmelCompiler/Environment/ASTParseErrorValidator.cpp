#include "Environment/ASTParseErrorValidator.hpp"
#include "Environment/ASTArgumentDefinitionNode.hpp"
#include "Environment/ASTCleanUpScopeNode.hpp"
#include "Environment/ASTClosureNode.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTLexicalScopeNode.hpp"
#include "Environment/ASTMakeAssociationNode.hpp"
#include "Environment/ASTMakeDictionaryNode.hpp"
#include "Environment/ASTMakeLiteralArrayNode.hpp"
#include "Environment/ASTMakeTupleNode.hpp"
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
#include "Environment/CompilationErrors.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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
    if(node->resultType)
        visitNode(node->resultType);
    visitNode(node->body);
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &)
{
    return AnyValuePtr();
}

AnyValuePtr ASTParseErrorValidator::visitCallNode(const ASTCallNodePtr &node)
{
    visitNode(node->function);
    for(const auto &arg : node->arguments)
        visitNode(arg);
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
        return parseErrorNodes.back()->asCompilationError();

    auto errors = basicMakeObject<CompilationErrors> ();
    for(const auto &node : parseErrorNodes)
        errors->errors.push_back(node->asCompilationError());

    return errors;
}

} // End of namespace Environment
} // End of namespace Sysmel