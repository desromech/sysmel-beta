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
    return visitor->visitCompileTimeConstantNode(shared_from_this());
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
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius