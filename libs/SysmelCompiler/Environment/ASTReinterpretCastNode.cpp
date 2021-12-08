#include "Environment/ASTReinterpretCastNode.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTReinterpretCastNode> ASTReinterpretCastNodeRegistration;

bool ASTReinterpretCastNode::isASTReinterpretCastNode() const
{
    return true;
}

AnyValuePtr ASTReinterpretCastNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitReinterpretCastNode(selfFromThis());
}

SExpression ASTReinterpretCastNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"reinterpretCast"}},
        sourcePosition->asSExpression(),
        expression->asSExpression(),
        targetType->asSExpression()}};
}

} // End of namespace Environment
} // End of namespace Sysmel