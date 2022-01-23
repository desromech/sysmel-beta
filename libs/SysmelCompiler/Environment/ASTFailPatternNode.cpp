#include "Environment/ASTFailPatternNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTFailPatternNode> ASTFailPatternNodeTypeRegistration;

bool ASTFailPatternNode::isASTFailPatternNode() const
{
    return true;
}

AnyValuePtr ASTFailPatternNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitFailPatternNode(selfFromThis());
}

SExpression ASTFailPatternNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"failPattern"}},
        sourcePosition->asSExpression()}};
}

} // End of namespace Environment
} // End of namespace Sysmel