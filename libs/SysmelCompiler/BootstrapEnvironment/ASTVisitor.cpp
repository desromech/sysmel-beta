#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/ASTCompileTimeEvaluationErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTParseErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticErrorNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTLocalVariableNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTGlobalVariableNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTFieldVariableNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTFunctionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMethodNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTEnumNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTStructNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTUnionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTClassNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTValueAsVoidTypeConversionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTUpcastTypeConversionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTDowncastTypeConversionNode.hpp"

#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTVisitor> ASTVisitorTypeRegistration;

AnyValuePtr ASTVisitor::visitNode(const ASTNodePtr &node)
{
    return node->accept(shared_from_this());
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

AnyValuePtr ASTVisitor::visitVariableAccessNode(const ASTVariableAccessNodePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitLocalImmutableAccessNode(const ASTLocalImmutableAccessNodePtr &)
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius