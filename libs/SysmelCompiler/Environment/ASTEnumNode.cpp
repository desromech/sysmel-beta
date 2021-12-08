#include "Environment/ASTEnumNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTEnumNode> ASTEnumNodeRegistration;

bool ASTEnumNode::isASTEnumNode() const
{
    return true;
}

AnyValuePtr ASTEnumNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitEnumNode(selfFromThis());
}

SExpression ASTEnumNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"enum"}},
        sourcePosition->asSExpression(),
        SExpressionIdentifier{{programEntityVisibilityToString(visibility)}},
        name ? name->asSExpression() : nullptr,
        valueType ? valueType->asSExpression() : nullptr,
        values ? values->asSExpression() : nullptr,
        body ? body->asSExpression() : nullptr,
    }};
}

void ASTEnumNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(valueType) aBlock(valueType);
    if(values) aBlock(values);
}

} // End of namespace Environment
} // End of namespace Sysmel