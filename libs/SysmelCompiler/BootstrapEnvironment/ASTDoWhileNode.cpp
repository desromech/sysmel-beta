#include "sysmel/BootstrapEnvironment/ASTDoWhileNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTDoWhileNode> ASTDoWhileNodeTypeRegistration;

bool ASTDoWhileNode::isASTDoWhileNode() const
{
    return true;
}

AnyValuePtr ASTDoWhileNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitDoWhileNode(shared_from_this());
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius