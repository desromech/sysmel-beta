#include "sysmel/ObjectModel/ASTQuoteNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius