#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTNode> ASTNodeTypeRegistration;

bool ASTNode::isASTNode() const
{
    return true;
}

AnyValuePtr ASTNode::encodeAsSExpression() const
{
    throw SubclassResponsibility();
}

std::string ASTNode::printString() const
{
    return encodeAsSExpression()->printString();
}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius