#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius