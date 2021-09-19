#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTMessageSendNode> ASTMessageSendNodeTypeRegistration;

bool ASTMessageSendNode::isASTMessageSendNode() const
{
    return true;
}

AnyValuePtr ASTMessageSendNode::encodeAsSExpression() const
{
    AnyValuePtrList result;
    result.reserve(1 + (receiver ? 1 : 0) +  arguments.size());
    result.push_back(internSymbol("send"));
    result.push_back(selector->encodeAsSExpression());
    result.push_back(receiver ? receiver->encodeAsSExpression() : getNilConstant());
    for(const auto &arg : arguments)
        result.push_back(arg->encodeAsSExpression());
    return wrapValue(result);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius