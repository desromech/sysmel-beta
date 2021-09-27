#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"
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
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitClosureNode(const ASTClosureNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitIntrinsicOperationNode(const ASTIntrinsicOperationNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitLexicalScopeNode(const ASTLexicalScopeNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitLiteralValueNode(const ASTLiteralValueNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMakeAssociationNode(const ASTMakeAssociationNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMakeTupleNode(const ASTMakeTupleNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMessageChainNode(const ASTMessageChainNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMessageChainMessageNode(const ASTMessageChainMessageNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitMessageSendNode(const ASTMessageSendNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitParseErrorNode(const ASTParseErrorNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitPragmaNode(const ASTPragmaNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitQuasiUnquoteNode(const ASTQuasiUnquoteNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitQuoteNode(const ASTQuoteNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitSequenceNode(const ASTSequenceNodePtr &)
{
    throw SubclassResponsibility();
}

AnyValuePtr ASTVisitor::visitSpliceNode(const ASTSpliceNodePtr &)
{
    throw SubclassResponsibility();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius