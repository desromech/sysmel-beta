#include "sysmel/ObjectModel/ASTNode.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ASTNode> ASTNodeTypeRegistration;

bool ASTNode::isASTNode() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius