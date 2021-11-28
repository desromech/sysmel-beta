#include "sysmel/BootstrapEnvironment/ASTIfNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTIfNode> ASTIfNodeTypeRegistration;

bool ASTIfNode::isASTIfNode() const
{
    return true;
}

AnyValuePtr ASTIfNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitIfNode(shared_from_this());
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius