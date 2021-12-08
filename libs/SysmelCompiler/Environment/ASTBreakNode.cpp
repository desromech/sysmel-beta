#include "Environment/ASTBreakNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTBreakNode> ASTBreakNodeTypeRegistration;

bool ASTBreakNode::isASTBreakNode() const
{
    return true;
}

AnyValuePtr ASTBreakNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitBreakNode(selfFromThis());
}

SExpression ASTBreakNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"break"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel