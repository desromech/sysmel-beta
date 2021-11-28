#include "sysmel/BootstrapEnvironment/ASTReturnNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTReturnNode> ASTReturnNodeTypeRegistration;

bool ASTReturnNode::isASTReturnNode() const
{
    return true;
}

AnyValuePtr ASTReturnNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitReturnNode(shared_from_this());
}

SExpression ASTReturnNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"return"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        expression ? expression->asSExpression() : nullptr,
    }};
}

void ASTReturnNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(expression) aBlock(expression);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius