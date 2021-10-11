#include "sysmel/BootstrapEnvironment/ASTFunctionalNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTFunctionalNode> ASTFunctionalNodeRegistration;

bool ASTFunctionalNode::isASTFunctionalNode() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius