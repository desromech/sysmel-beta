#include "sysmel/BootstrapEnvironment/ASTMessageChainNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainMessageNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTMessageChainNode> ASTMessageChainNodeTypeRegistration;

bool ASTMessageChainNode::isASTMessageChainNode() const
{
    return true;
}

AnyValuePtr ASTMessageChainNode::encodeAsSExpression() const
{
    AnyValuePtrList result;
    result.reserve(1 + (receiver ? 1 : 0) +  messages.size());
    result.push_back(internSymbol("chain"));
    result.push_back(receiver ? receiver->encodeAsSExpression() : getNilConstant());
    for(const auto &message : messages)
        result.push_back(message->encodeAsSExpression());
    return wrapValue(result);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius