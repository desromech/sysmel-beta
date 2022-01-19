#include "Environment/ASTValuePatternNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTValuePatternNode> ASTValuePatternNodeTypeRegistration;

bool ASTValuePatternNode::isASTValuePatternNode() const
{
    return true;
}

AnyValuePtr ASTValuePatternNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitValuePatternNode(selfFromThis());
}

SExpression ASTValuePatternNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"valuePattern"}},
        sourcePosition->asSExpression(),
        expectedValue->asSExpression()
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel