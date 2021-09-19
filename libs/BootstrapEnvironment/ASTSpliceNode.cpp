#include "sysmel/BootstrapEnvironment/ASTSpliceNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTSpliceNode> ASTSpliceNodeTypeRegistration;

bool ASTSpliceNode::isASTSpliceNode() const
{
    return true;
}

AnyValuePtr ASTSpliceNode::encodeAsSExpression() const
{
    return wrapValue(AnyValuePtrList{internSymbol("splice"), expression->encodeAsSExpression()});
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius