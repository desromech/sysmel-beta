#include "sysmel/BootstrapEnvironment/ASTQuasiQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius