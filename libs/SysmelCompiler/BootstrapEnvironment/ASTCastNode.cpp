#include "sysmel/BootstrapEnvironment/ASTCastNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTCastNode> ASTCastNodeRegistration;

bool ASTCastNode::isASTCastNode() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius