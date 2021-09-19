#include "sysmel/ObjectModel/ASTArgumentDefinitionNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ASTArgumentDefinitionNode> ASTArgumentDefinitionNodeTypeRegistration;

bool ASTArgumentDefinitionNode::isASTArgumentDefinitionNode() const
{
    return true;
}

AnyValuePtr ASTArgumentDefinitionNode::encodeAsSExpression() const
{
    return wrapValue(AnyValuePtrList{internSymbol("argument"),
        identifier ? identifier->encodeAsSExpression() : getNilConstant(),
        type ? type->encodeAsSExpression() : getNilConstant()});
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius