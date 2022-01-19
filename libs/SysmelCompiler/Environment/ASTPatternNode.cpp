#include "Environment/ASTPatternNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTPatternNode> ASTPatternNodeTypeRegistration;

bool ASTPatternNode::isASTPatternNode() const
{
    return true;
}

ASTNodePtr ASTPatternNode::parseAsPatternNode()
{
    return selfFromThis();
}

} // End of namespace Environment
} // End of namespace Sysmel