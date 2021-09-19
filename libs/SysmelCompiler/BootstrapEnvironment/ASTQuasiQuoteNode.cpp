#include "sysmel/BootstrapEnvironment/ASTQuasiQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTQuasiQuoteNode> ASTQuasiQuoteNodeTypeRegistration;

bool ASTQuasiQuoteNode::isASTQuasiQuoteNode() const
{
    return true;
}

AnyValuePtr ASTQuasiQuoteNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitQuasiQuoteNode(shared_from_this());
}

SExpression ASTQuasiQuoteNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"quasiQuote"}},
        sourcePosition->asSExpression(),
        expression->asSExpression()
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius