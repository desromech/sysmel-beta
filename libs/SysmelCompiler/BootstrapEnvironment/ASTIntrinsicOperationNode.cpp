#include "sysmel/BootstrapEnvironment/ASTIntrinsicOperationNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
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

AnyValuePtr ASTIntrinsicOperationNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitIntrinsicOperationNode(shared_from_this());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius