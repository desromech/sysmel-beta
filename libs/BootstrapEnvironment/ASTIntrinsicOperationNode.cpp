#include "sysmel/BootstrapEnvironment/ASTIntrinsicOperationNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTIntrinsicOperationNode> ASTIntrinsicOperationNodeTypeRegistration;

bool ASTIntrinsicOperationNode::isASTIntrinsicOperationNode() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius