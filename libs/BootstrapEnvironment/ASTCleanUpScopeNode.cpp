#include "sysmel/BootstrapEnvironment/ASTCleanUpScopeNode.hpp"
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

AnyValuePtr ASTCleanUpScopeNode::encodeAsSExpression() const
{
    return wrapValue(AnyValuePtrList{internSymbol("cleanUpScope"), body ? body->encodeAsSExpression() : getNilConstant()});
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius