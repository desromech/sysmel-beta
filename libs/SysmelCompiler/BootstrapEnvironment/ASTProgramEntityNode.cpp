#include "sysmel/BootstrapEnvironment/ASTProgramEntityNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTProgramEntityNode> ASTProgramEntityNodeRegistration;

bool ASTProgramEntityNode::isASTProgramEntityNode() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius