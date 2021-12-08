#include "Environment/ASTTypeNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTTypeNode> ASTTypeNodeRegistration;

bool ASTTypeNode::isASTTypeNode() const
{
    return true;
}

void ASTTypeNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(name) aBlock(name);
    if(body) aBlock(body);
    if(alignment) aBlock(alignment);
}

} // End of namespace Environment
} // End of namespace Sysmel