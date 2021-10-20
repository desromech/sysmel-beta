#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_COMPILE_TIME_EVALUATOR_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_COMPILE_TIME_EVALUATOR_HPP
#pragma once

#include "ASTVisitor.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompileTimeCleanUpScope)

/**
 * I am a compile time evaluator of a previously analyzed AST.
 */
class ASTCompileTimeEvaluator : public SubtypeOf<ASTVisitor, ASTCompileTimeEvaluator>
{
public:
    static constexpr char const __typeName__[] = "ASTCompileTimeEvaluator";

    AnyValuePtr visitNodeInNewCleanUpScope(const ASTNodePtr &node);

    virtual AnyValuePtr visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node) override;
    virtual AnyValuePtr visitClosureNode(const ASTClosureNodePtr &node) override;
    virtual AnyValuePtr visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node) override;
    virtual AnyValuePtr visitCallNode(const ASTCallNodePtr &node) override;
    virtual AnyValuePtr visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node) override;
    virtual AnyValuePtr visitLiteralValueNode(const ASTLiteralValueNodePtr &node) override;
    virtual AnyValuePtr visitMakeAssociationNode(const ASTMakeAssociationNodePtr &node) override;
    virtual AnyValuePtr visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node) override;
    virtual AnyValuePtr visitMakeTupleNode(const ASTMakeTupleNodePtr &node) override;
    virtual AnyValuePtr visitMessageSendNode(const ASTMessageSendNodePtr &node) override;
    virtual AnyValuePtr visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &node) override;
    virtual AnyValuePtr visitQuoteNode(const ASTQuoteNodePtr &node) override;
    virtual AnyValuePtr visitSequenceNode(const ASTSequenceNodePtr &node) override;

    virtual AnyValuePtr visitLocalVariableNode(const ASTLocalVariableNodePtr &node) override;
    virtual AnyValuePtr visitVariableAccessNode(const ASTVariableAccessNodePtr &node) override;
    virtual AnyValuePtr visitLocalImmutableAccessNode(const ASTLocalImmutableAccessNodePtr &node) override;

    virtual AnyValuePtr visitFunctionalNode(const ASTFunctionalNodePtr &node) override;
    virtual AnyValuePtr visitNamespaceNode(const ASTNamespaceNodePtr &node) override;
    virtual AnyValuePtr visitTypeNode(const ASTTypeNodePtr &node) override;

    AnyValuePtr evaluateMethodBodyNode(const CompileTimeCleanUpScopePtr &initialEnvironment, const ASTNodePtr &node);

    CompileTimeCleanUpScopePtr currentCleanUpScope;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_COMPILE_TIME_EVALUATOR_HPP