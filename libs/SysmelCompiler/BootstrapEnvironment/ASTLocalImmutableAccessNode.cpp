#include "sysmel/BootstrapEnvironment/ASTLocalImmutableAccessNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTLocalImmutableAccessNode> ASTLocalImmutableAccessNodeRegistration;

bool ASTLocalImmutableAccessNode::isASTLocalImmutableAccessNode() const
{
    return true;
}

AnyValuePtr ASTLocalImmutableAccessNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitLocalImmutableAccessNode(shared_from_this());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius