#include "Environment/ASTUnionNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTUnionNode> ASTUnionNodeRegistration;

bool ASTUnionNode::isASTUnionNode() const
{
    return true;
}

AnyValuePtr ASTUnionNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitUnionNode(selfFromThis());
}

SExpression ASTUnionNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"struct"}},
        sourcePosition->asSExpression(),
        SExpressionIdentifier{{programEntityVisibilityToString(visibility)}},
        name ? name->asSExpression() : nullptr,
        body ? body->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel