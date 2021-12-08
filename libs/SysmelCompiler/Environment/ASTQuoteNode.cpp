#include "Environment/ASTQuoteNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTQuoteNode> ASTQuoteNodeTypeRegistration;

bool ASTQuoteNode::isASTQuoteNode() const
{
    return true;
}

AnyValuePtr ASTQuoteNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitQuoteNode(selfFromThis());
}

SExpression ASTQuoteNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"quote"}},
        analyzedType ? analyzedType->asSExpression() : nullptr,
        sourcePosition->asSExpression(),
        expression->asSExpression()
    }};
}

void ASTQuoteNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(expression) aBlock(expression);
}

} // End of namespace Environment
} // End of namespace Sysmel