#include "sysmel/BootstrapEnvironment/ASTSpliceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTSpliceNode> ASTSpliceNodeTypeRegistration;

bool ASTSpliceNode::isASTSpliceNode() const
{
    return true;
}
AnyValuePtr ASTSpliceNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitSpliceNode(shared_from_this());
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius