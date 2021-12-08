#include "Environment/ASTReturnNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTReturnNode> ASTReturnNodeTypeRegistration;

bool ASTReturnNode::isASTReturnNode() const
{
    return true;
}

AnyValuePtr ASTReturnNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitReturnNode(selfFromThis());
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

} // End of namespace Environment
} // End of namespace Sysmel