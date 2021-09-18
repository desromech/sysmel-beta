#include "sysmel/ObjectModel/ASTParseErrorNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius