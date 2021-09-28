#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTNode> ASTNodeTypeRegistration;

ASTNode::ASTNode()
{
    sourcePosition = ASTSourcePosition::empty();
}

AnyValuePtr ASTNode::accept(const ASTVisitorPtr &)
{
    SysmelSelfSubclassResponsibility();
}

bool ASTNode::isASTNode() const
{
    return true;
}

std::string ASTNode::printString() const
{
    return sexpressionToPrettyString(asSExpression());
}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius