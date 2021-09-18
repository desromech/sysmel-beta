#include "sysmel/ObjectModel/ASTMessageSendNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius