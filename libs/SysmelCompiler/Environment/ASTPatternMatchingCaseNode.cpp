#include "Environment/ASTPatternMatchingCaseNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTPatternMatchingCaseNode> ASTPatternMatchingCaseNodeTypeRegistration;

bool ASTPatternMatchingCaseNode::isASTPatternMatchingCaseNode() const
{
    return true;
}

AnyValuePtr ASTPatternMatchingCaseNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitPatternMatchingCaseNode(selfFromThis());
}

SExpression ASTPatternMatchingCaseNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"patternMatchingCase"}},
        sourcePosition->asSExpression(),

        patternNode ? patternNode->asSExpression() : nullptr,
        actionNode ? actionNode->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel