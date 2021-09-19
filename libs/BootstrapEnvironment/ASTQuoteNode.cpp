#include "sysmel/BootstrapEnvironment/ASTQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTQuoteNode> ASTQuoteNodeTypeRegistration;

bool ASTQuoteNode::isASTQuoteNode() const
{
    return true;
}

SExpression ASTQuoteNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"quote"}},
        sourcePosition->asSExpression(),
        expression->asSExpression()
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius