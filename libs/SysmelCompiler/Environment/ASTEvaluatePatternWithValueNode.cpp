#include "Environment/ASTEvaluatePatternWithValueNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTEvaluatePatternWithValueNode> ASTEvaluatePatternWithValueNodeTypeRegistration;

bool ASTEvaluatePatternWithValueNode::isASTEvaluatePatternWithValueNode() const
{
    return true;
}

AnyValuePtr ASTEvaluatePatternWithValueNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitEvaluatePatternWithValueNode(selfFromThis());
}

SExpression ASTEvaluatePatternWithValueNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"evaluatePatternWithValue"}},
        sourcePosition->asSExpression(),
    
        introduceNewLexicalScope,
        patternNode ? patternNode->asSExpression() : nullptr,
        valueNode ? valueNode->asSExpression() : nullptr,
        patternEvaluationNode ? patternEvaluationNode->asSExpression() : nullptr,
        successAction ? successAction->asSExpression() : nullptr,
        failureAction ? failureAction->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel