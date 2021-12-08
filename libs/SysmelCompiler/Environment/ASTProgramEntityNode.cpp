#include "Environment/ASTProgramEntityNode.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTProgramEntityNode> ASTProgramEntityNodeRegistration;

bool ASTProgramEntityNode::isASTProgramEntityNode() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel