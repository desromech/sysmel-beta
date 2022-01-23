#include "Environment/ASTDestructuringBindingNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTDestructuringBindingNode> ASTDestructuringBindingNodeTypeRegistration;

bool ASTDestructuringBindingNode::isASTDestructuringBindingNode() const
{
    return true;
}

AnyValuePtr ASTDestructuringBindingNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitDestructuringBindingNode(selfFromThis());
}

SExpression ASTDestructuringBindingNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"destructuringBinding"}},
        sourcePosition->asSExpression(),
    
        patternNode ? patternNode->asSExpression() : nullptr,
        valueNode ? valueNode->asSExpression() : nullptr
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel