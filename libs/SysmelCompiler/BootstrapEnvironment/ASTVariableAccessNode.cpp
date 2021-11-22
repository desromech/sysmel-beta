#include "sysmel/BootstrapEnvironment/ASTVariableAccessNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/Variable.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTVariableAccessNode> ASTVariableAccessNodeRegistration;

bool ASTVariableAccessNode::isASTVariableAccessNode() const
{
    return true;
}

AnyValuePtr ASTVariableAccessNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitVariableAccessNode(shared_from_this());
}

SExpression ASTVariableAccessNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"variableAccess"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        variable->asSExpression()
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius