#include "sysmel/BootstrapEnvironment/ASTExplicitCastNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTExplicitCastNode> ASTExplicitCastNodeRegistration;

bool ASTExplicitCastNode::isASTExplicitCastNode() const
{
    return true;
}

AnyValuePtr ASTExplicitCastNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitExplicitCastNode(shared_from_this());
}

SExpression ASTExplicitCastNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"explicitCast"}},
        sourcePosition->asSExpression(),
        expression->asSExpression(),
        targetType->asSExpression()}};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius