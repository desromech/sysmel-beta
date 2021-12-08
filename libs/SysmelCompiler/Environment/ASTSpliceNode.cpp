#include "Environment/ASTSpliceNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTSpliceNode> ASTSpliceNodeTypeRegistration;

bool ASTSpliceNode::isASTSpliceNode() const
{
    return true;
}
AnyValuePtr ASTSpliceNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitSpliceNode(selfFromThis());
}

SExpression ASTSpliceNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"splice"}},
        analyzedType ? analyzedType->asSExpression() : nullptr,
        sourcePosition->asSExpression(),
        expression->asSExpression()
    }};
}

void ASTSpliceNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(expression) aBlock(expression);
}

} // End of namespace Environment
} // End of namespace Sysmel