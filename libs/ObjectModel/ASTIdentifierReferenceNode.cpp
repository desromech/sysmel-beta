#include "sysmel/ObjectModel/ASTIdentifierReferenceNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius