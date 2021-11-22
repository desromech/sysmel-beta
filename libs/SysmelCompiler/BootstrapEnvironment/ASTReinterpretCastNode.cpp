#include "sysmel/BootstrapEnvironment/ASTReinterpretCastNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTReinterpretCastNode> ASTReinterpretCastNodeRegistration;

bool ASTReinterpretCastNode::isASTReinterpretCastNode() const
{
    return true;
}

AnyValuePtr ASTReinterpretCastNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitReinterpretCastNode(shared_from_this());
}

SExpression ASTReinterpretCastNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"reinterpretCast"}},
        sourcePosition->asSExpression(),
        expression->asSExpression(),
        targetType->asSExpression()}};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius