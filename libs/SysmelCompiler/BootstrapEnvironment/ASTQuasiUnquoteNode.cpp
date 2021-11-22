#include "sysmel/BootstrapEnvironment/ASTQuasiUnquoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTQuasiUnquoteNode> ASTQuasiUnquoteNodeTypeRegistration;

bool ASTQuasiUnquoteNode::isASTQuasiUnquoteNode() const
{
    return true;
}

AnyValuePtr ASTQuasiUnquoteNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitQuasiUnquoteNode(shared_from_this());
}

SExpression ASTQuasiUnquoteNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"quasiUnquote"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        expression->asSExpression()
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius