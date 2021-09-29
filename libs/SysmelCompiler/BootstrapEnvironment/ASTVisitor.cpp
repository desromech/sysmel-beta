#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/ASTCompileTimeEvaluationErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTParseErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticErrorNode.hpp"
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

AnyValuePtr ASTVisitor::visitIntrinsicOperationNode(const ASTIntrinsicOperationNodePtr &)
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius