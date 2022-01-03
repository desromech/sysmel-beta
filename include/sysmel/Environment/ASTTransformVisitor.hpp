#ifndef SYSMEL_ENVIRONMENT_AST_TRANSFORM_VISITOR_HPP
#define SYSMEL_ENVIRONMENT_AST_TRANSFORM_VISITOR_HPP
#pragma once

#include "ASTVisitor.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTTransformVisitor : public SubtypeOf<ASTVisitor, ASTTransformVisitor>
{
public:
    static constexpr char const __typeName__[] = "ASTTransformVisitor";

    void transformChildNode(ASTNodePtr& childNode);
    virtual void transformChildNodeList(ASTNodePtrList& childNodeList);

    virtual AnyValuePtr visitArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &node) override;
    virtual AnyValuePtr visitCallNode(const ASTCallNodePtr &node) override;
    virtual AnyValuePtr visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node) override;
    virtual AnyValuePtr visitClosureNode(const ASTClosureNodePtr &node) override;
    virtual AnyValuePtr visitCompileTimeEvaluationErrorNode(const ASTCompileTimeEvaluationErrorNodePtr &node) override;
    virtual AnyValuePtr visitErrorNode(const ASTErrorNodePtr &node) override;
    virtual AnyValuePtr visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node) override;
    virtual AnyValuePtr visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node) override;
    virtual AnyValuePtr visitLiteralValueNode(const ASTLiteralValueNodePtr &node) override;
    virtual AnyValuePtr visitMakeAggregateNode(const ASTMakeAggregateNodePtr &node) override;
    virtual AnyValuePtr visitMakeAssociationNode(const ASTMakeAssociationNodePtr &node) override;
    virtual AnyValuePtr visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node) override;
    virtual AnyValuePtr visitMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &node) override;
    virtual AnyValuePtr visitMakeTupleNode(const ASTMakeTupleNodePtr &node) override;
    virtual AnyValuePtr visitMakeVariantNode(const ASTMakeVariantNodePtr &node) override;
    virtual AnyValuePtr visitMakeVectorNode(const ASTMakeVectorNodePtr &node) override;
    virtual AnyValuePtr visitMessageChainNode(const ASTMessageChainNodePtr &node) override;
    virtual AnyValuePtr visitMessageChainMessageNode(const ASTMessageChainMessageNodePtr &node) override;
    virtual AnyValuePtr visitMessageSendNode(const ASTMessageSendNodePtr &node) override;
    virtual AnyValuePtr visitParseErrorNode(const ASTParseErrorNodePtr &node) override;
    virtual AnyValuePtr visitPragmaNode(const ASTPragmaNodePtr &node) override;
    virtual AnyValuePtr visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &node) override;
    virtual AnyValuePtr visitQuasiUnquoteNode(const ASTQuasiUnquoteNodePtr &node) override;
    virtual AnyValuePtr visitQuoteNode(const ASTQuoteNodePtr &node) override;
    virtual AnyValuePtr visitSemanticErrorNode(const ASTSemanticErrorNodePtr &node) override;
    virtual AnyValuePtr visitSequenceNode(const ASTSequenceNodePtr &node) override;
    virtual AnyValuePtr visitSpliceNode(const ASTSpliceNodePtr &node) override;
    virtual AnyValuePtr visitVectorSwizzleNode(const ASTVectorSwizzleNodePtr &node) override;

    virtual AnyValuePtr visitQuasiQuotePatternExpansionNode(const ASTQuasiQuotePatternExpansionNodePtr &node) override;
    virtual AnyValuePtr visitQuasiQuotePatternExpansionArgumentNode(const ASTQuasiQuotePatternExpansionArgumentNodePtr &node) override;

    virtual AnyValuePtr visitProgramEntityNode(const ASTProgramEntityNodePtr &node) override;
    virtual AnyValuePtr visitVariableNode(const ASTVariableNodePtr &node) override;
    virtual AnyValuePtr visitLocalVariableNode(const ASTLocalVariableNodePtr &node) override;
    virtual AnyValuePtr visitGlobalVariableNode(const ASTGlobalVariableNodePtr &node) override;
    virtual AnyValuePtr visitFieldVariableNode(const ASTFieldVariableNodePtr &node) override;
    virtual AnyValuePtr visitCompileTimeConstantNode(const ASTCompileTimeConstantNodePtr &node) override;

    virtual AnyValuePtr visitFieldVariableAccessNode(const ASTFieldVariableAccessNodePtr &node) override;
    virtual AnyValuePtr visitVariableAccessNode(const ASTVariableAccessNodePtr &node) override;
    virtual AnyValuePtr visitLocalImmutableAccessNode(const ASTLocalImmutableAccessNodePtr &node) override;
    virtual AnyValuePtr visitSlotAccessNode(const ASTSlotAccessNodePtr &node) override;

    virtual AnyValuePtr visitFunctionalNode(const ASTFunctionalNodePtr &node) override;
    virtual AnyValuePtr visitFunctionNode(const ASTFunctionNodePtr &node) override;
    virtual AnyValuePtr visitMethodNode(const ASTMethodNodePtr &node) override;

    virtual AnyValuePtr visitNamespaceNode(const ASTNamespaceNodePtr &node) override;

    virtual AnyValuePtr visitTypeNode(const ASTTypeNodePtr &node) override;
    virtual AnyValuePtr visitEnumNode(const ASTEnumNodePtr &node) override;
    virtual AnyValuePtr visitStructNode(const ASTStructNodePtr &node) override;
    virtual AnyValuePtr visitUnionNode(const ASTUnionNodePtr &node) override;
    virtual AnyValuePtr visitClassNode(const ASTClassNodePtr &node) override;
    virtual AnyValuePtr visitTemplateNode(const ASTTemplateNodePtr &node) override;
    virtual AnyValuePtr visitProgramEntityExtensionNode(const ASTProgramEntityExtensionNodePtr &node) override;

    virtual AnyValuePtr visitCastNode(const ASTCastNodePtr &node) override;
    virtual AnyValuePtr visitExplicitCastNode(const ASTExplicitCastNodePtr &node) override;
    virtual AnyValuePtr visitImplicitCastNode(const ASTImplicitCastNodePtr &node) override;
    virtual AnyValuePtr visitReinterpretCastNode(const ASTReinterpretCastNodePtr &node) override;

    virtual AnyValuePtr visitTypeConversionNode(const ASTTypeConversionNodePtr &node) override;
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
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_TRANSFORM_VISITOR_HPP