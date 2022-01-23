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

bool ASTAnyValuePatternNode::isAlwaysMatchingPattern() const
{
    return true;
}

ASTNodePtr ASTAnyValuePatternNode::optimizePatternNodeForExpectedTypeWith(const TypePtr &type, const ASTSemanticAnalyzerPtr &)
{
    auto result = shallowCloneObject(selfFromThis());
    result->analyzedType = type;
    return result;
}

ASTNodePtr ASTAnyValuePatternNode::expandPatternNodeForExpectedTypeWith(const TypePtr &, const ASTNodePtr &patternValueNode, const ASTSemanticAnalyzerPtr &)
{
    return patternValueNode;
}

} // End of namespace Environment
} // End of namespace Sysmel