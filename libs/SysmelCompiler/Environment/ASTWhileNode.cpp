#include "Environment/ASTWhileNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTWhileNode> ASTWhileNodeTypeRegistration;

bool ASTWhileNode::isASTWhileNode() const
{
    return true;
}

AnyValuePtr ASTWhileNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitWhileNode(selfFromThis());
}

SExpression ASTWhileNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"while"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        condition ? condition->asSExpression() : nullptr,
        bodyExpression ? bodyExpression->asSExpression() : nullptr,
        continueExpression ? continueExpression->asSExpression() : nullptr,
    }};
}

void ASTWhileNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(condition) aBlock(condition);
    if(bodyExpression) aBlock(bodyExpression);
    if(continueExpression) aBlock(continueExpression);
}

} // End of namespace Environment
} // End of namespace Sysmel