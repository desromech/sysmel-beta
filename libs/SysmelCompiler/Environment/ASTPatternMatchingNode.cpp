#include "Environment/ASTPatternMatchingNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTPatternMatchingNode> ASTPatternMatchingNodeTypeRegistration;

bool ASTPatternMatchingNode::isASTPatternMatchingNode() const
{
    return true;
}

AnyValuePtr ASTPatternMatchingNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitPatternMatchingNode(selfFromThis());
}

SExpression ASTPatternMatchingNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"patternMatching"}},
        sourcePosition->asSExpression(),

        valueNode ? valueNode->asSExpression() : nullptr,

        patternNodes ? patternNodes->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel