#include "Environment/ASTBindingPatternNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTBindingPatternNode> ASTBindingPatternNodeTypeRegistration;

bool ASTBindingPatternNode::isASTBindingPatternNode() const
{
    return true;
}

AnyValuePtr ASTBindingPatternNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitBindingPatternNode(selfFromThis());
}

SExpression ASTBindingPatternNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"bindingPattern"}},
        sourcePosition->asSExpression(),

        identifier->asSExpression(),
        expectedType ? expectedType->asSExpression() : nullptr,
        expectedValue ? expectedValue->asSExpression() : nullptr
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel