#include "Environment/ASTImplicitCastNode.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTImplicitCastNode> ASTImplicitCastNodeRegistration;

bool ASTImplicitCastNode::isASTImplicitCastNode() const
{
    return true;
}

AnyValuePtr ASTImplicitCastNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitImplicitCastNode(selfFromThis());
}

SExpression ASTImplicitCastNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"implicitCast"}},
        sourcePosition->asSExpression(),
        expression->asSExpression(),
        targetType->asSExpression()}};
}

} // End of namespace Environment
} // End of namespace Sysmel