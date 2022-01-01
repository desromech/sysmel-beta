#ifndef SYSMEL_ENVIRONMENT_AST_COMPILE_TIME_EVALUATOR_HPP
#define SYSMEL_ENVIRONMENT_AST_COMPILE_TIME_EVALUATOR_HPP
#pragma once

#include "ASTVisitor.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompileTimeCleanUpScope)

/**
 * I am a compile time evaluator of a previously analyzed AST.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTCompileTimeEvaluator : public SubtypeOf<ASTVisitor, ASTCompileTimeEvaluator>
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
    virtual AnyValuePtr visitMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &node) override;
    virtual AnyValuePtr visitMakeTupleNode(const ASTMakeTupleNodePtr &node) override;
    virtual AnyValuePtr visitMakeVectorNode(const ASTMakeVectorNodePtr &node) override;
    virtual AnyValuePtr visitMessageSendNode(const ASTMessageSendNodePtr &node) override;
    virtual AnyValuePtr visitQuoteNode(const ASTQuoteNodePtr &node) override;
    virtual AnyValuePtr visitSequenceNode(const ASTSequenceNodePtr &node) override;
    virtual AnyValuePtr visitVectorSwizzleNode(const ASTVectorSwizzleNodePtr &node) override;

    virtual AnyValuePtr visitQuasiQuotePatternExpansionNode(const ASTQuasiQuotePatternExpansionNodePtr &node) override;

    virtual AnyValuePtr visitLocalVariableNode(const ASTLocalVariableNodePtr &node) override;
    virtual AnyValuePtr visitGlobalVariableNode(const ASTGlobalVariableNodePtr &node) override;
    virtual AnyValuePtr visitFieldVariableAccessNode(const ASTFieldVariableAccessNodePtr &node) override;
    virtual AnyValuePtr visitVariableAccessNode(const ASTVariableAccessNodePtr &node) override;
    virtual AnyValuePtr visitLocalImmutableAccessNode(const ASTLocalImmutableAccessNodePtr &node) override;

    virtual AnyValuePtr visitProgramEntityNode(const ASTProgramEntityNodePtr &node) override;
    virtual AnyValuePtr visitFunctionalNode(const ASTFunctionalNodePtr &node) override;
    virtual AnyValuePtr visitNamespaceNode(const ASTNamespaceNodePtr &node) override;
    virtual AnyValuePtr visitTypeNode(const ASTTypeNodePtr &node) override;
    virtual AnyValuePtr visitProgramEntityExtensionNode(const ASTProgramEntityExtensionNodePtr &node) override;

    virtual AnyValuePtr visitValueAsVoidTypeConversionNode(const ASTValueAsVoidTypeConversionNodePtr &node) override;
    virtual AnyValuePtr visitUpcastTypeConversionNode(const ASTUpcastTypeConversionNodePtr &node) override;
    virtual AnyValuePtr visitDowncastTypeConversionNode(const ASTDowncastTypeConversionNodePtr &node) override;
    virtual AnyValuePtr visitValueAsReferenceReinterpretConversionNode(const ASTValueAsReferenceReinterpretConversionNodePtr &node) override;

    virtual AnyValuePtr visitIfNode(const ASTIfNodePtr &node) override;
    virtual AnyValuePtr visitWhileNode(const ASTWhileNodePtr &node) override;
    virtual AnyValuePtr visitDoWhileNode(const ASTDoWhileNodePtr &node) override;
    virtual AnyValuePtr visitReturnNode(const ASTReturnNodePtr &node) override;
    virtual AnyValuePtr visitContinueNode(const ASTContinueNodePtr &node) override;
    virtual AnyValuePtr visitBreakNode(const ASTBreakNodePtr &node) override;

    AnyValuePtr evaluateMethodBodyNode(const CompileTimeCleanUpScopePtr &initialEnvironment, const ASTNodePtr &node);
    AnyValuePtr visitNodeCachingExplicitReturns(const ASTNodePtr &node);

    CompileTimeCleanUpScopePtr currentCleanUpScope;
    void *currentReturnToken = nullptr;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_COMPILE_TIME_EVALUATOR_HPP