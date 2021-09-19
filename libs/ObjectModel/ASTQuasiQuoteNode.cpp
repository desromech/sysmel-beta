#include "sysmel/ObjectModel/ASTQuasiQuoteNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius