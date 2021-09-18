#include "sysmel/ObjectModel/ASTNode.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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


} // End of namespace ObjectModel
} // End of namespace SysmelMoebius