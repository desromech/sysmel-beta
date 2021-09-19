#include "sysmel/BootstrapEnvironment/ASTClosureNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTClosureNode> ASTClosureNodeTypeRegistration;

bool ASTClosureNode::isASTClosureNode() const
{
    return true;
}

AnyValuePtr ASTClosureNode::encodeAsSExpression() const
{
    AnyValuePtrList result;
    result.reserve(4);
    result.push_back(internSymbol("closure"));

    AnyValuePtrList resultArguments;
    for(const auto &arg : arguments )
        resultArguments.push_back(arg->encodeAsSExpression());
    result.push_back(wrapValue(resultArguments));
    result.push_back(returnType ? returnType->encodeAsSExpression() : getNilConstant());
    result.push_back(body->encodeAsSExpression());
    return wrapValue(result);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius