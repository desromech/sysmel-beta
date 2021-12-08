#include "Environment/ASTCompileTimeConstantNode.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTCompileTimeConstantNode> ASTCompileTimeConstantNodeRegistration;

bool ASTCompileTimeConstantNode::isASTCompileTimeConstantNode() const
{
    return true;
}

AnyValuePtr ASTCompileTimeConstantNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitCompileTimeConstantNode(selfFromThis());
}

SExpression ASTCompileTimeConstantNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"compileTimeConstant"}},
        sourcePosition->asSExpression(),
        name ? name->asSExpression() : nullptr,
        type ? type->asSExpression() : nullptr,
        value ? value->asSExpression() : nullptr,
    }};
}

void ASTCompileTimeConstantNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(name) aBlock(name);
    if(type) aBlock(type);
    if(value) aBlock(value);
}

} // End of namespace Environment
} // End of namespace Sysmel