#include "sysmel/BootstrapEnvironment/ASTCallNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTCallNode> ASTCallNodeTypeRegistration;

bool ASTCallNode::isASTCallNode() const
{
    return true;
}

AnyValuePtr ASTCallNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitCallNode(shared_from_this());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius