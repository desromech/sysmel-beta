#include "sysmel/BootstrapEnvironment/ASTSequenceNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTSequenceNode> ASTSequenceNodeTypeRegistration;

bool ASTSequenceNode::isASTSequenceNode() const
{
    return true;
}

AnyValuePtr ASTSequenceNode::encodeAsSExpression() const
{
    AnyValuePtrList resultPragmas;
    resultPragmas.reserve(pragmas.size());
    for(const auto &pragma : pragmas)
        resultPragmas.push_back(pragma->encodeAsSExpression());

    AnyValuePtrList result;
    result.reserve(2 + expressions.size());
    result.push_back(internSymbol("sequence"));
    result.push_back(wrapValue(resultPragmas));
    for(const auto &expr : expressions)
        result.push_back(expr->encodeAsSExpression());
    return wrapValue(result);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius