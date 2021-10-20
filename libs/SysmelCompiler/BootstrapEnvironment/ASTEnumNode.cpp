#include "sysmel/BootstrapEnvironment/ASTEnumNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTEnumNode> ASTEnumNodeRegistration;

bool ASTEnumNode::isASTEnumNode() const
{
    return true;
}

AnyValuePtr ASTEnumNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitEnumNode(shared_from_this());
}

SExpression ASTEnumNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"enum"}},
        sourcePosition->asSExpression(),
        SExpressionIdentifier{{programEntityVisibilityToString(visibility)}},
        name ? name->asSExpression() : nullptr,
        valueType ? valueType->asSExpression() : nullptr,
        values ? values->asSExpression() : nullptr,
        body ? body->asSExpression() : nullptr,
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius