#include "sysmel/ObjectModel/ASTMessageChainMessageNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ASTMessageChainMessageNode> ASTMessageChainMessageNodeTypeRegistration;

bool ASTMessageChainMessageNode::isASTMessageChainMessageNode() const
{
    return true;
}

AnyValuePtr ASTMessageChainMessageNode::encodeAsSExpression() const
{
    AnyValuePtrList result;
    result.reserve(1 + 1 +  arguments.size());
    result.push_back(internSymbol("chainedMessage"));
    result.push_back(selector->encodeAsSExpression());
    for(const auto &arg : arguments)
        result.push_back(arg->encodeAsSExpression());
    return wrapValue(result);
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius