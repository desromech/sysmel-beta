#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius