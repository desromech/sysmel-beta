#include "sysmel/BootstrapEnvironment/ASTStructNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTStructNode> ASTStructNodeRegistration;

bool ASTStructNode::isASTStructNode() const
{
    return true;
}

AnyValuePtr ASTStructNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitStructNode(selfFromThis());
}

SExpression ASTStructNode::asSExpression() const
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