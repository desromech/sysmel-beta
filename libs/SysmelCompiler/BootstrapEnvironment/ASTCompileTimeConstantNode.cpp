#include "sysmel/BootstrapEnvironment/ASTCompileTimeConstantNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius