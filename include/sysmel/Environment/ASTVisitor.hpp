#ifndef SYSMEL_ENVIRONMENT_AST_VISITOR_HPP
#define SYSMEL_ENVIRONMENT_AST_VISITOR_HPP
#pragma once

#include "ASTNode.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTVisitor : public SubtypeOf<CompilerObject, ASTVisitor>
{
public:
    static constexpr char const __typeName__[] = "ASTVisitor";

    virtual AnyValuePtr visitNode(const ASTNodePtr &node);
    virtual AnyValuePtr visitArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &node);
    virtual AnyValuePtr visitCallNode(const ASTCallNodePtr &node);
    virtual AnyValuePtr visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node);
    virtual AnyValuePtr visitClosureNode(const ASTClosureNodePtr &node);
    virtual AnyValuePtr visitCompileTimeEvaluationErrorNode(const ASTCompileTimeEvaluationErrorNodePtr &node);
    virtual AnyValuePtr visitErrorNode(const ASTErrorNodePtr &node);
    virtual AnyValuePtr visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node);
    virtual AnyValuePtr visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node);
    virtual AnyValuePtr visitLiteralValueNode(const ASTLiteralValueNodePtr &node);
    virtual AnyValuePtr visitMakeAggregateNode(const ASTMakeAggregateNodePtr &node);
    virtual AnyValuePtr visitMakeAssociationNode(const ASTMakeAssociationNodePtr &node);
    virtual AnyValuePtr visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node);
    virtual AnyValuePtr visitMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &node);
    virtual AnyValuePtr visitMakeTupleNode(const ASTMakeTupleNodePtr &node);
    virtual AnyValuePtr visitMakeVectorNode(const ASTMakeVectorNodePtr &node);
    virtual AnyValuePtr visitMessageChainNode(const ASTMessageChainNodePtr &node);
    virtual AnyValuePtr visitMessageChainMessageNode(const ASTMessageChainMessageNodePtr &node);
    virtual AnyValuePtr visitMessageSendNode(const ASTMessageSendNodePtr &node);
    virtual AnyValuePtr visitParseErrorNode(const ASTParseErrorNodePtr &node);
    virtual AnyValuePtr visitPragmaNode(const ASTPragmaNodePtr &node);
    virtual AnyValuePtr visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &node);
    virtual AnyValuePtr visitQuasiUnquoteNode(const ASTQuasiUnquoteNodePtr &node);
    virtual AnyValuePtr visitQuoteNode(const ASTQuoteNodePtr &node);
    virtual AnyValuePtr visitSemanticErrorNode(const ASTSemanticErrorNodePtr &node);
    virtual AnyValuePtr visitSequenceNode(const ASTSequenceNodePtr &node);
    virtual AnyValuePtr visitSpliceNode(const ASTSpliceNodePtr &node);
    virtual AnyValuePtr visitVectorSwizzleNode(const ASTVectorSwizzleNodePtr &node);

    virtual AnyValuePtr visitQuasiQuotePatternExpansionNode(const ASTQuasiQuotePatternExpansionNodePtr &node);
    virtual AnyValuePtr visitQuasiQuotePatternExpansionArgumentNode(const ASTQuasiQuotePatternExpansionArgumentNodePtr &node);

    virtual AnyValuePtr visitProgramEntityNode(const ASTProgramEntityNodePtr &node);
    virtual AnyValuePtr visitVariableNode(const ASTVariableNodePtr &node);
    virtual AnyValuePtr visitLocalVariableNode(const ASTLocalVariableNodePtr &node);
    virtual AnyValuePtr visitGlobalVariableNode(const ASTGlobalVariableNodePtr &node);
    virtual AnyValuePtr visitFieldVariableNode(const ASTFieldVariableNodePtr &node);
    virtual AnyValuePtr visitCompileTimeConstantNode(const ASTCompileTimeConstantNodePtr &node);

    virtual AnyValuePtr visitFieldVariableAccessNode(const ASTFieldVariableAccessNodePtr &node);
    virtual AnyValuePtr visitVariableAccessNode(const ASTVariableAccessNodePtr &node);
    virtual AnyValuePtr visitLocalImmutableAccessNode(const ASTLocalImmutableAccessNodePtr &node);
    virtual AnyValuePtr visitSlotAccessNode(const ASTSlotAccessNodePtr &node);

    virtual AnyValuePtr visitFunctionalNode(const ASTFunctionalNodePtr &node);
    virtual AnyValuePtr visitFunctionNode(const ASTFunctionNodePtr &node);
    virtual AnyValuePtr visitMethodNode(const ASTMethodNodePtr &node);

    virtual AnyValuePtr visitNamespaceNode(const ASTNamespaceNodePtr &node);

    virtual AnyValuePtr visitTypeNode(const ASTTypeNodePtr &node);
    virtual AnyValuePtr visitEnumNode(const ASTEnumNodePtr &node);
    virtual AnyValuePtr visitStructNode(const ASTStructNodePtr &node);
    virtual AnyValuePtr visitUnionNode(const ASTUnionNodePtr &node);
    virtual AnyValuePtr visitClassNode(const ASTClassNodePtr &node);
    virtual AnyValuePtr visitTemplateNode(const ASTTemplateNodePtr &node);
    virtual AnyValuePtr visitProgramEntityExtensionNode(const ASTProgramEntityExtensionNodePtr &node);

    virtual AnyValuePtr visitCastNode(const ASTCastNodePtr &node);
    virtual AnyValuePtr visitExplicitCastNode(const ASTExplicitCastNodePtr &node);
    virtual AnyValuePtr visitImplicitCastNode(const ASTImplicitCastNodePtr &node);
    virtual AnyValuePtr visitReinterpretCastNode(const ASTReinterpretCastNodePtr &node);

    virtual AnyValuePtr visitTypeConversionNode(const ASTTypeConversionNodePtr &node);
    virtual AnyValuePtr visitValueAsVoidTypeConversionNode(const ASTValueAsVoidTypeConversionNodePtr &node);
    virtual AnyValuePtr visitUpcastTypeConversionNode(const ASTUpcastTypeConversionNodePtr &node);
    virtual AnyValuePtr visitDowncastTypeConversionNode(const ASTDowncastTypeConversionNodePtr &node);
    virtual AnyValuePtr visitValueAsReferenceReinterpretConversionNode(const ASTValueAsReferenceReinterpretConversionNodePtr &node);

    virtual AnyValuePtr visitIfNode(const ASTIfNodePtr &node);
    virtual AnyValuePtr visitWhileNode(const ASTWhileNodePtr &node);
    virtual AnyValuePtr visitDoWhileNode(const ASTDoWhileNodePtr &node);
    virtual AnyValuePtr visitReturnNode(const ASTReturnNodePtr &node);
    virtual AnyValuePtr visitContinueNode(const ASTContinueNodePtr &node);
    virtual AnyValuePtr visitBreakNode(const ASTBreakNodePtr &node);
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_VISITOR_HPP