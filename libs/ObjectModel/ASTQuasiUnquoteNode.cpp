#include "sysmel/ObjectModel/ASTQuasiUnquoteNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius