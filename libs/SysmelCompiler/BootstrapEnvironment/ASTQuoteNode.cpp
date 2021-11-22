#include "sysmel/BootstrapEnvironment/ASTQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
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

AnyValuePtr ASTQuoteNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitQuoteNode(shared_from_this());
}

SExpression ASTQuoteNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"quote"}},
        analyzedType ? analyzedType->asSExpression() : nullptr,
        sourcePosition->asSExpression(),
        expression->asSExpression()
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius