#include "sysmel/BootstrapEnvironment/ASTQuasiQuoteNode.hpp"
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

AnyValuePtr ASTQuasiQuoteNode::encodeAsSExpression() const
{
    return wrapValue(AnyValuePtrList{internSymbol("quasiQuote"), expression->encodeAsSExpression()});
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius