#include "Environment/ASTMakeVariantNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTMakeVariantNode> ASTMakeVariantNodeTypeRegistration;

bool ASTMakeVariantNode::isASTMakeVariantNode() const
{
    return true;
}

AnyValuePtr ASTMakeVariantNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMakeVariantNode(selfFromThis());
}

SExpression ASTMakeVariantNode::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"variant"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        variantType->asSExpression(),
        value->asSExpression()
    }};
}

void ASTMakeVariantNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    aBlock(variantType);
    aBlock(value);
}

} // End of namespace Environment
} // End of namespace Sysmel