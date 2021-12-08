#include "Environment/ASTQuasiQuoteNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTQuasiQuoteNode> ASTQuasiQuoteNodeTypeRegistration;

bool ASTQuasiQuoteNode::isASTQuasiQuoteNode() const
{
    return true;
}

AnyValuePtr ASTQuasiQuoteNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitQuasiQuoteNode(selfFromThis());
}

SExpression ASTQuasiQuoteNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"quasiQuote"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        expression->asSExpression()
    }};
}

void ASTQuasiQuoteNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(expression) aBlock(expression);
}

} // End of namespace Environment
} // End of namespace Sysmel