#include "Environment/ASTAnyValuePatternNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTAnyValuePatternNode> ASTAnyValuePatternNodeTypeRegistration;

bool ASTAnyValuePatternNode::isASTAnyValuePatternNode() const
{
    return true;
}

AnyValuePtr ASTAnyValuePatternNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitAnyValuePatternNode(selfFromThis());
}

SExpression ASTAnyValuePatternNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"anyValuePattern"}},
        sourcePosition->asSExpression(),
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel