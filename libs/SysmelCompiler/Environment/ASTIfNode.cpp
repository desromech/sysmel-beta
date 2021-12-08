#include "Environment/ASTIfNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTIfNode> ASTIfNodeTypeRegistration;

bool ASTIfNode::isASTIfNode() const
{
    return true;
}

AnyValuePtr ASTIfNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitIfNode(selfFromThis());
}

SExpression ASTIfNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"if"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        condition->asSExpression(),
        trueExpression ? trueExpression->asSExpression() : nullptr,
        falseExpression ? falseExpression->asSExpression() : nullptr,
    }};
}

void ASTIfNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(condition) aBlock(condition);
    if(trueExpression) aBlock(trueExpression);
    if(falseExpression) aBlock(falseExpression);
}

} // End of namespace Environment
} // End of namespace Sysmel