#include "sysmel/BootstrapEnvironment/ASTIdentifierReferenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTIdentifierReferenceNode> ASTIdentifierReferenceNodeTypeRegistration;

bool ASTIdentifierReferenceNode::isASTIdentifierReferenceNode() const
{
    return true;
}

AnyValuePtr ASTIdentifierReferenceNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitIdentifierReferenceNode(shared_from_this());
}

SExpression ASTIdentifierReferenceNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"identifier"}},
        sourcePosition->asSExpression(),
        identifier->asSExpression()}};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius