#include "sysmel/BootstrapEnvironment/ASTQuoteNode.hpp"
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

AnyValuePtr ASTQuoteNode::encodeAsSExpression() const
{
    return wrapValue(AnyValuePtrList{internSymbol("quote"), expression->encodeAsSExpression()});
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius