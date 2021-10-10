#include "sysmel/BootstrapEnvironment/ASTVariableAccessNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTVariableAccessNode> ASTVariableAccessNodeRegistration;

bool ASTVariableAccessNode::isASTVariableAccessNode() const
{
    return true;
}

AnyValuePtr ASTVariableAccessNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitVariableAccessNode(shared_from_this());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius