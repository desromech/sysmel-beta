#include "sysmel/BootstrapEnvironment/ASTVariableNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius