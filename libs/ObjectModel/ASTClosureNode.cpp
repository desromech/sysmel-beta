#include "sysmel/ObjectModel/ASTClosureNode.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ASTClosureNode> ASTClosureNodeTypeRegistration;

bool ASTClosureNode::isASTClosureNode() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius