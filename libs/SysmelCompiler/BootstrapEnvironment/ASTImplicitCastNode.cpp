#include "sysmel/BootstrapEnvironment/ASTImplicitCastNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTImplicitCastNode> ASTImplicitCastNodeRegistration;

bool ASTImplicitCastNode::isASTImplicitCastNode() const
{
    return true;
}

AnyValuePtr ASTImplicitCastNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitImplicitCastNode(shared_from_this());
}

SExpression ASTImplicitCastNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"implicitCast"}},
        sourcePosition->asSExpression(),
        expression->asSExpression(),
        targetType->asSExpression()}};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius