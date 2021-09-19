#include "sysmel/BootstrapEnvironment/ASTParseErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTParseErrorNode> ASTParseErrorNodeTypeRegistration;

bool ASTParseErrorNode::isASTParseErrorNode() const
{
    return true;
}

AnyValuePtr ASTParseErrorNode::encodeAsSExpression() const
{
    return wrapValue(AnyValuePtrList{internSymbol("parseError"), wrapValue(errorMessage)});
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius