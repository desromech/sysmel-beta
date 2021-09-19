#include "sysmel/BootstrapEnvironment/ASTPragmaNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTPragmaNode> ASTPragmaNodeTypeRegistration;

bool ASTPragmaNode::isASTPragmaNode() const
{
    return true;
}

AnyValuePtr ASTPragmaNode::encodeAsSExpression() const
{
    AnyValuePtrList result;
    result.reserve(1 + arguments.size());
    result.push_back(internSymbol("pragma"));
    result.push_back(selector->encodeAsSExpression());
    for(const auto &arg : arguments)
        result.push_back(arg->encodeAsSExpression());
    return wrapValue(result);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius