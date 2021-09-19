#include "sysmel/ObjectModel/ASTSpliceNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ASTSpliceNode> ASTSpliceNodeTypeRegistration;

bool ASTSpliceNode::isASTSpliceNode() const
{
    return true;
}

AnyValuePtr ASTSpliceNode::encodeAsSExpression() const
{
    return wrapValue(AnyValuePtrList{internSymbol("splice"), expression->encodeAsSExpression()});
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius