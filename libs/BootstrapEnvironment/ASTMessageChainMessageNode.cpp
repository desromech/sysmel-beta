#include "sysmel/BootstrapEnvironment/ASTMessageChainMessageNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius