#include "sysmel/BootstrapEnvironment/ASTMethodNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTMethodNode> ASTMethodNodeTypeRegistration;

bool ASTMethodNode::isASTMethodNode() const
{
    return true;
}

AnyValuePtr ASTMethodNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitMethodNode(selfFromThis());
}

SExpression ASTMethodNode::asSExpression() const
{
    SExpressionList argumentsSExpr;
    argumentsSExpr.elements.reserve(arguments.size());
    for(const auto &arg : arguments)
        argumentsSExpr.elements.push_back(arg->asSExpression());

    return SExpressionList{{SExpressionIdentifier{{"method"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        SExpressionIdentifier{{programEntityVisibilityToString(visibility)}},
        SExpressionIdentifier{{methodDispatchModeToString(dispatchMode)}},
        name ? name->asSExpression() : nullptr,
        argumentsSExpr,
        resultType ? resultType->asSExpression() : nullptr,
        body ? body->asSExpression() : nullptr,
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius