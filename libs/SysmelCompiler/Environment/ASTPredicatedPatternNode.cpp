#include "Environment/ASTPredicatedPatternNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTPredicatedPatternNode> ASTPredicatedPatternNodeTypeRegistration;

bool ASTPredicatedPatternNode::isASTPredicatedPatternNode() const
{
    return true;
}

AnyValuePtr ASTPredicatedPatternNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitPredicatedPatternNode(selfFromThis());
}

SExpression ASTPredicatedPatternNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"predicatedPattern"}},
        sourcePosition->asSExpression(),
        expectedPattern->asSExpression(),
        predicate->asSExpression()
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel