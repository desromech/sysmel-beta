#include "Environment/ASTVariableNode.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTVariableNode> ASTVariableNodeRegistration;

bool ASTVariableNode::isASTVariableNode() const
{
    return true;
}

void ASTVariableNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(name) aBlock(name);
    if(type) aBlock(type);
    if(alignment) aBlock(alignment);
    if(initialValue) aBlock(initialValue);
}

} // End of namespace Environment
} // End of namespace Sysmel