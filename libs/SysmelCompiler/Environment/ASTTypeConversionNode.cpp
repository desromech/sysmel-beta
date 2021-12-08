#include "Environment/ASTTypeConversionNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel