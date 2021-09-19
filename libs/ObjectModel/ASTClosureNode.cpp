#include "sysmel/ObjectModel/ASTClosureNode.hpp"
#include "sysmel/ObjectModel/ASTArgumentDefinitionNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius