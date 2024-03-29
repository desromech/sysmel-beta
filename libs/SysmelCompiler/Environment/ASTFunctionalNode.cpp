#include "Environment/ASTFunctionalNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTFunctionalNode> ASTFunctionalNodeRegistration;

bool ASTFunctionalNode::isASTFunctionalNode() const
{
    return true;
}

bool ASTFunctionalNode::isRegisteredInLexicalScope() const
{
    return visibility == ProgramEntityVisibility::LexicalScope;
}

void ASTFunctionalNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(name) aBlock(name);
    for(auto &arg : arguments)
        aBlock(arg);
    if(resultType) aBlock(resultType);
    if(body) aBlock(body);
}

} // End of namespace Environment
} // End of namespace Sysmel