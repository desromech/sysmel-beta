#include "Environment/ASTVisitor.hpp"
#include "Environment/ASTCompileTimeEvaluationErrorNode.hpp"
#include "Environment/ASTParseErrorNode.hpp"
#include "Environment/ASTSemanticErrorNode.hpp"

#include "Environment/ASTLocalVariableNode.hpp"
#include "Environment/ASTGlobalVariableNode.hpp"
#include "Environment/ASTFieldVariableNode.hpp"
#include "Environment/ASTCompileTimeConstantNode.hpp"

#include "Environment/ASTFunctionNode.hpp"
#include "Environment/ASTMethodNode.hpp"

#include "Environment/ASTEnumNode.hpp"
#include "Environment/ASTStructNode.hpp"
#include "Environment/ASTUnionNode.hpp"
#include "Environment/ASTClassNode.hpp"
#include "Environment/ASTTemplateNode.hpp"

#include "Environment/ASTExplicitCastNode.hpp"
#include "Environment/ASTImplicitCastNode.hpp"
#include "Environment/ASTReinterpretCastNode.hpp"

#include "Environment/ASTValueAsVoidTypeConversionNode.hpp"
#include "Environment/ASTUpcastTypeConversionNode.hpp"
#include "Environment/ASTDowncastTypeConversionNode.hpp"
#include "Environment/ASTValueAsReferenceReinterpretConversionNode.hpp"

#include "Environment/ASTAlternativesPatternNode.hpp"
#include "Environment/ASTAnyValuePatternNode.hpp"
#include "Environment/ASTBindingPatternNode.hpp"
#include "Environment/ASTNeverPatternNode.hpp"
#include "Environment/ASTPredicatedPatternNode.hpp"
#include "Environment/ASTRangePatternNode.hpp"
#include "Environment/ASTSequencePatternNode.hpp"
#include "Environment/ASTValuePatternNode.hpp"

#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTVisitor> ASTVisitorTypeRegistration;

AnyValuePtr ASTVisitor::visitNode(const ASTNodePtr &node)
{
    return node->accept(selfFromThis());
}

AnyValuePtr ASTVisitor::visitArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitClosureNode(const ASTClosureNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitCompileTimeEvaluationErrorNode(const ASTCompileTimeEvaluationErrorNodePtr &node)
{
    return visitErrorNode(node);
}

AnyValuePtr ASTVisitor::visitErrorNode(const ASTErrorNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitCallNode(const ASTCallNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitLexicalScopeNode(const ASTLexicalScopeNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitLiteralValueNode(const ASTLiteralValueNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMakeAggregateNode(const ASTMakeAggregateNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMakeAssociationNode(const ASTMakeAssociationNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMakeTupleNode(const ASTMakeTupleNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMakeVariantNode(const ASTMakeVariantNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMakeVectorNode(const ASTMakeVectorNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMessageChainNode(const ASTMessageChainNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMessageChainMessageNode(const ASTMessageChainMessageNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMessageSendNode(const ASTMessageSendNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitParseErrorNode(const ASTParseErrorNodePtr &node)
{
    return visitErrorNode(node);
}

AnyValuePtr ASTVisitor::visitPragmaNode(const ASTPragmaNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitQuasiUnquoteNode(const ASTQuasiUnquoteNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitQuoteNode(const ASTQuoteNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitSemanticErrorNode(const ASTSemanticErrorNodePtr &node)
{
    return visitErrorNode(node);
}

AnyValuePtr ASTVisitor::visitSequenceNode(const ASTSequenceNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitSpliceNode(const ASTSpliceNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitVectorSwizzleNode(const ASTVectorSwizzleNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitQuasiQuotePatternExpansionNode(const ASTQuasiQuotePatternExpansionNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitQuasiQuotePatternExpansionArgumentNode(const ASTQuasiQuotePatternExpansionArgumentNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitProgramEntityNode(const ASTProgramEntityNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitVariableNode(const ASTVariableNodePtr &node)
{
    return visitProgramEntityNode(node);
}

AnyValuePtr ASTVisitor::visitLocalVariableNode(const ASTLocalVariableNodePtr &node)
{
    return visitVariableNode(node);
}

AnyValuePtr ASTVisitor::visitGlobalVariableNode(const ASTGlobalVariableNodePtr &node)
{
    return visitVariableNode(node);
}

AnyValuePtr ASTVisitor::visitFieldVariableNode(const ASTFieldVariableNodePtr &node)
{
    return visitVariableNode(node);
}

AnyValuePtr ASTVisitor::visitCompileTimeConstantNode(const ASTCompileTimeConstantNodePtr &node)
{
    return visitProgramEntityNode(node);
}

AnyValuePtr ASTVisitor::visitFieldVariableAccessNode(const ASTFieldVariableAccessNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitVariableAccessNode(const ASTVariableAccessNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitLocalImmutableAccessNode(const ASTLocalImmutableAccessNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitSlotAccessNode(const ASTSlotAccessNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitFunctionalNode(const ASTFunctionalNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitFunctionNode(const ASTFunctionNodePtr &node)
{
    return visitFunctionalNode(node);
}
AnyValuePtr ASTVisitor::visitMethodNode(const ASTMethodNodePtr &node)
{
    return visitFunctionalNode(node);
}

AnyValuePtr ASTVisitor::visitNamespaceNode(const ASTNamespaceNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitTypeNode(const ASTTypeNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitEnumNode(const ASTEnumNodePtr &node)
{
    return visitTypeNode(node);
}

AnyValuePtr ASTVisitor::visitStructNode(const ASTStructNodePtr &node)
{
    return visitTypeNode(node);
}

AnyValuePtr ASTVisitor::visitUnionNode(const ASTUnionNodePtr &node)
{
    return visitTypeNode(node);
}

AnyValuePtr ASTVisitor::visitClassNode(const ASTClassNodePtr &node)
{
    return visitTypeNode(node);
}

AnyValuePtr ASTVisitor::visitTemplateNode(const ASTTemplateNodePtr &node)
{
    return visitProgramEntityNode(node);
}

AnyValuePtr ASTVisitor::visitProgramEntityExtensionNode(const ASTProgramEntityExtensionNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitCastNode(const ASTCastNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitExplicitCastNode(const ASTExplicitCastNodePtr &node)
{
    return visitCastNode(node);
}

AnyValuePtr ASTVisitor::visitImplicitCastNode(const ASTImplicitCastNodePtr &node)
{
    return visitCastNode(node);
}

AnyValuePtr ASTVisitor::visitReinterpretCastNode(const ASTReinterpretCastNodePtr &node)
{
    return visitCastNode(node);
}

AnyValuePtr ASTVisitor::visitTypeConversionNode(const ASTTypeConversionNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitValueAsVoidTypeConversionNode(const ASTValueAsVoidTypeConversionNodePtr &node)
{
    return visitTypeConversionNode(node);
}

AnyValuePtr ASTVisitor::visitUpcastTypeConversionNode(const ASTUpcastTypeConversionNodePtr &node)
{
    return visitTypeConversionNode(node);
}

AnyValuePtr ASTVisitor::visitDowncastTypeConversionNode(const ASTDowncastTypeConversionNodePtr &node)
{
    return visitTypeConversionNode(node);
}

AnyValuePtr ASTVisitor::visitValueAsReferenceReinterpretConversionNode(const ASTValueAsReferenceReinterpretConversionNodePtr &node)
{
    return visitTypeConversionNode(node);
}

AnyValuePtr ASTVisitor::visitIfNode(const ASTIfNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitWhileNode(const ASTWhileNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitDoWhileNode(const ASTDoWhileNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitReturnNode(const ASTReturnNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitContinueNode(const ASTContinueNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitBreakNode(const ASTBreakNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitPatternNode(const ASTPatternNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitAlternativesPatternNode(const ASTAlternativesPatternNodePtr &node)
{
    return visitPatternNode(node);
}

AnyValuePtr ASTVisitor::visitAnyValuePatternNode(const ASTAnyValuePatternNodePtr &node)
{
    return visitPatternNode(node);
}

AnyValuePtr ASTVisitor::visitBindingPatternNode(const ASTBindingPatternNodePtr &node)
{
    return visitPatternNode(node);
}

AnyValuePtr ASTVisitor::visitNeverPatternNode(const ASTNeverPatternNodePtr &node)
{
    return visitPatternNode(node);
}

AnyValuePtr ASTVisitor::visitPredicatedPatternNode(const ASTPredicatedPatternNodePtr &node)
{
    return visitPatternNode(node);
}

AnyValuePtr ASTVisitor::visitRangePatternNode(const ASTRangePatternNodePtr &node)
{
    return visitPatternNode(node);
}

AnyValuePtr ASTVisitor::visitSequencePatternNode(const ASTSequencePatternNodePtr &node)
{
    return visitPatternNode(node);
}

AnyValuePtr ASTVisitor::visitValuePatternNode(const ASTValuePatternNodePtr &node)
{
    return visitPatternNode(node);
}

AnyValuePtr ASTVisitor::visitDestructuringBindingNode(const ASTDestructuringBindingNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitPatternMatchingNode(const ASTPatternMatchingNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitPatternMatchingCaseNode(const ASTPatternMatchingCaseNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitEvaluatePatternWithValueNode(const ASTEvaluatePatternWithValueNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitFailPatternNode(const ASTFailPatternNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitTrapNode(const ASTTrapNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

} // End of namespace Environment
} // End of namespace Sysmel
