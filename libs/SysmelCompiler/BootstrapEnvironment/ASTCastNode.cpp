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

void ASTCastNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(expression) aBlock(expression);
    if(targetType) aBlock(targetType);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius