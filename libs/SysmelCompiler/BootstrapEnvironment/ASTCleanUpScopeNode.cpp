#include "sysmel/BootstrapEnvironment/ASTCleanUpScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTCleanUpScopeNode> ASTCleanUpScopeNodeRegistration;

bool ASTCleanUpScopeNode::isASTCleanUpScopeNode() const
{
    return true;
}

AnyValuePtr ASTCleanUpScopeNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitCleanUpScopeNode(shared_from_this());
}

SExpression ASTCleanUpScopeNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"cleanUpScope"}},
        sourcePosition->asSExpression(),
        body->asSExpression()
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius