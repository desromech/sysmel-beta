#include "sysmel/BootstrapEnvironment/ASTTypeNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTTypeNode> ASTTypeNodeRegistration;

bool ASTTypeNode::isASTTypeNode() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius