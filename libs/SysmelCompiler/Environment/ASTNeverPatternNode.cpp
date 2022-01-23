#include "Environment/ASTNeverPatternNode.hpp"
#include "Environment/ASTFailPatternNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTNeverPatternNode> ASTNeverPatternNodeTypeRegistration;

bool ASTNeverPatternNode::isASTNeverPatternNode() const
{
    return true;
}

AnyValuePtr ASTNeverPatternNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitNeverPatternNode(selfFromThis());
}

SExpression ASTNeverPatternNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"neverPatternNode"}},
        sourcePosition->asSExpression(),
    }};
}

bool ASTNeverPatternNode::isNeverMatchingPattern() const
{
    return true;
}

ASTNeverPatternNodePtr ASTNeverPatternNode::asNeverPatternNode()
{
    return selfFromThis();
}

ASTNodePtr ASTNeverPatternNode::expandPatternNodeForExpectedTypeWith(const TypePtr &, const ASTNodePtr &, const ASTSemanticAnalyzerPtr &)
{
    auto result = basicMakeObject<ASTFailPatternNode> ();
    result->sourcePosition = sourcePosition;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel