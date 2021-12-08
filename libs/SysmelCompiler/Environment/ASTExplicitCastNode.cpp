#include "Environment/ASTExplicitCastNode.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTExplicitCastNode> ASTExplicitCastNodeRegistration;

bool ASTExplicitCastNode::isASTExplicitCastNode() const
{
    return true;
}

AnyValuePtr ASTExplicitCastNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitExplicitCastNode(selfFromThis());
}

SExpression ASTExplicitCastNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"explicitCast"}},
        sourcePosition->asSExpression(),
        expression->asSExpression(),
        targetType->asSExpression()}};
}

} // End of namespace Environment
} // End of namespace Sysmel