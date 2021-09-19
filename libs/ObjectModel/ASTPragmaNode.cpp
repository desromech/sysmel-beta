#include "sysmel/ObjectModel/ASTPragmaNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius