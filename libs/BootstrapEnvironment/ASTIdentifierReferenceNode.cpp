#include "sysmel/BootstrapEnvironment/ASTIdentifierReferenceNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTIdentifierReferenceNode> ASTIdentifierReferenceNodeTypeRegistration;

bool ASTIdentifierReferenceNode::isASTIdentifierReferenceNode() const
{
    return true;
}

AnyValuePtr ASTIdentifierReferenceNode::encodeAsSExpression() const
{
    return wrapValue(AnyValuePtrList{internSymbol("identifier"), identifier});
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius