#include "sysmel/ObjectModel/ASTIntrinsicOperationNode.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ASTIntrinsicOperationNode> ASTIntrinsicOperationNodeTypeRegistration;

bool ASTIntrinsicOperationNode::isASTIntrinsicOperationNode() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius