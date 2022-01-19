#include "Environment/ASTRangePatternNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTRangePatternNode> ASTRangePatternNodeTypeRegistration;

bool ASTRangePatternNode::isASTRangePatternNode() const
{
    return true;
}

AnyValuePtr ASTRangePatternNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitRangePatternNode(selfFromThis());
}

SExpression ASTRangePatternNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"rangePattern"}},
        sourcePosition->asSExpression(),
        startNode->asSExpression(),
        endNode->asSExpression(),
        isInclusive
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel