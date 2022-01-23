#include "Environment/ASTTrapNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTTrapNode> ASTTrapNodeTypeRegistration;

bool ASTTrapNode::isASTTrapNode() const
{
    return true;
}

AnyValuePtr ASTTrapNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitTrapNode(selfFromThis());
}

SExpression ASTTrapNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"trap"}},
        sourcePosition->asSExpression(),

        reasonMessage}};
}

} // End of namespace Environment
} // End of namespace Sysmel