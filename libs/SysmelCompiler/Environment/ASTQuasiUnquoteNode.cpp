#include "Environment/ASTQuasiUnquoteNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTQuasiUnquoteNode> ASTQuasiUnquoteNodeTypeRegistration;

bool ASTQuasiUnquoteNode::isASTQuasiUnquoteNode() const
{
    return true;
}

AnyValuePtr ASTQuasiUnquoteNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitQuasiUnquoteNode(selfFromThis());
}

SExpression ASTQuasiUnquoteNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"quasiUnquote"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        expression->asSExpression()
    }};
}

void ASTQuasiUnquoteNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(expression) aBlock(expression);
}

} // End of namespace Environment
} // End of namespace Sysmel