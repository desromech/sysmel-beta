#include "sysmel/ObjectModel/ASTSequenceNode.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<ASTSequenceNode> ASTSequenceNodeTypeRegistration;

bool ASTSequenceNode::isASTSequenceNode() const
{
    return true;
}

AnyValuePtr ASTSequenceNode::encodeAsSExpression() const
{
    AnyValuePtrList result;
    result.reserve(1 + expressions.size());
    result.push_back(internSymbol("sequence"));
    for(const auto &expr : expressions)
        result.push_back(expr->encodeAsSExpression());
    return wrapValue(result);
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius