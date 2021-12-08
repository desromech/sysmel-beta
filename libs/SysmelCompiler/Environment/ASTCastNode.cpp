#include "Environment/ASTCastNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTCastNode> ASTCastNodeRegistration;

bool ASTCastNode::isASTCastNode() const
{
    return true;
}

void ASTCastNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(expression) aBlock(expression);
    if(targetType) aBlock(targetType);
}

} // End of namespace Environment
} // End of namespace Sysmel