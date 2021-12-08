#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PARSE_ERROR_VALIDATOR_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_PARSE_ERROR_VALIDATOR_HPP
#pragma once

#include "ASTVisitor.hpp"
#include "CompilationError.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTParseErrorValidator : public SubtypeOf<ASTVisitor, ASTParseErrorValidator>
{
public:
    static constexpr char const __typeName__[] = "ASTParseErrorValidator";

    virtual AnyValuePtr visitArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &node);
    virtual AnyValuePtr visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node);
    virtual AnyValuePtr visitClosureNode(const ASTClosureNodePtr &node);
    virtual AnyValuePtr visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node);
    virtual AnyValuePtr visitCallNode(const ASTCallNodePtr &node);
    virtual AnyValuePtr visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node);
    virtual AnyValuePtr visitLiteralValueNode(const ASTLiteralValueNodePtr &node);
    virtual AnyValuePtr visitMakeAssociationNode(const ASTMakeAssociationNodePtr &node);
    virtual AnyValuePtr visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node);
    virtual AnyValuePtr visitMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &node);
    virtual AnyValuePtr visitMakeTupleNode(const ASTMakeTupleNodePtr &node);
    virtual AnyValuePtr visitMessageChainNode(const ASTMessageChainNodePtr &node);
    virtual AnyValuePtr visitMessageChainMessageNode(const ASTMessageChainMessageNodePtr &node);
    virtual AnyValuePtr visitMessageSendNode(const ASTMessageSendNodePtr &node);
    virtual AnyValuePtr visitParseErrorNode(const ASTParseErrorNodePtr &node);
    virtual AnyValuePtr visitPragmaNode(const ASTPragmaNodePtr &node);
    virtual AnyValuePtr visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &node);
    virtual AnyValuePtr visitQuasiUnquoteNode(const ASTQuasiUnquoteNodePtr &node);
    virtual AnyValuePtr visitQuoteNode(const ASTQuoteNodePtr &node);
    virtual AnyValuePtr visitSequenceNode(const ASTSequenceNodePtr &node);
    virtual AnyValuePtr visitSpliceNode(const ASTSpliceNodePtr &node);

    CompilationErrorPtr makeCompilationError();
    std::vector<ASTParseErrorNodePtr> parseErrorNodes;

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEMANTIC_ANALYZER_HPP