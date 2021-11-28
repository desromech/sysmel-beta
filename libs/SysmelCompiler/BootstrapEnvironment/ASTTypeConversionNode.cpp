#include "sysmel/BootstrapEnvironment/ASTTypeConversionNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTTypeConversionNode> ASTTypeConversionNodeRegistration;

bool ASTTypeConversionNode::isASTTypeConversionNode() const
{
    return true;
}

void ASTTypeConversionNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(expression) aBlock(expression);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius