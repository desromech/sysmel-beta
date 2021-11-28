#include "sysmel/BootstrapEnvironment/ASTTypeNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius