#include "sysmel/ObjectModel/ASTCleanUpScopeNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ASTCleanUpScopeNode> ASTCleanUpScopeNodeRegistration;

bool ASTCleanUpScopeNode::isASTCleanUpScopeNode() const
{
    return true;
}

AnyValuePtr ASTCleanUpScopeNode::encodeAsSExpression() const
{
    return wrapValue(AnyValuePtrList{internSymbol("cleanUpScope"), body ? body->encodeAsSExpression() : getNilConstant()});
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius