#include "sysmel/BootstrapEnvironment/ASTQuasiUnquoteNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTQuasiUnquoteNode> ASTQuasiUnquoteNodeTypeRegistration;

bool ASTQuasiUnquoteNode::isASTQuasiUnquoteNode() const
{
    return true;
}

AnyValuePtr ASTQuasiUnquoteNode::encodeAsSExpression() const
{
    return wrapValue(AnyValuePtrList{internSymbol("quasiUnquote"), expression->encodeAsSExpression()});
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius