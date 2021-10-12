#include "sysmel/BootstrapEnvironment/ASTFunctionalNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius