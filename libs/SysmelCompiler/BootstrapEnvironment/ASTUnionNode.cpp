#include "sysmel/BootstrapEnvironment/ASTUnionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTUnionNode> ASTUnionNodeRegistration;

bool ASTUnionNode::isASTUnionNode() const
{
    return true;
}

AnyValuePtr ASTUnionNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitUnionNode(shared_from_this());
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius