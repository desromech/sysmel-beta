#include "sysmel/ObjectModel/ASTLiteralValueNode.hpp"
#include "sysmel/ObjectModel/Type.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ASTLiteralValueNode> ASTLiteralValueNodeTypeRegistration;

bool ASTLiteralValueNode::isASTLiteralValueNode() const
{
    return true;
}

void ASTLiteralValueNode::setValueAndType(const AnyValuePtr &theValue)
{
    value = theValue;
    type = theValue->getType();
}

AnyValuePtr ASTLiteralValueNode::encodeAsSExpression() const
{
    return wrapValue(AnyValuePtrList{internSymbol("literal"), value, type});
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius