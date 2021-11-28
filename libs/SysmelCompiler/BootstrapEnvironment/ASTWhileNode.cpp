#include "sysmel/BootstrapEnvironment/ASTWhileNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius