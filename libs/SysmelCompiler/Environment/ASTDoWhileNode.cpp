#include "Environment/ASTDoWhileNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTDoWhileNode> ASTDoWhileNodeTypeRegistration;

bool ASTDoWhileNode::isASTDoWhileNode() const
{
    return true;
}

AnyValuePtr ASTDoWhileNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitDoWhileNode(selfFromThis());
}

SExpression ASTDoWhileNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"doWhile"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        bodyExpression ? bodyExpression->asSExpression() : nullptr,
        condition ? condition->asSExpression() : nullptr,
        continueExpression ? continueExpression->asSExpression() : nullptr,
    }};
}

void ASTDoWhileNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(bodyExpression) aBlock(bodyExpression);
    if(condition) aBlock(condition);
    if(continueExpression) aBlock(continueExpression);
}

} // End of namespace Environment
} // End of namespace Sysmel