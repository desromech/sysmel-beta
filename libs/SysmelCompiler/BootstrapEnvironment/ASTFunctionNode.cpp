#include "sysmel/BootstrapEnvironment/ASTFunctionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTFunctionNode> ASTFunctionNodeTypeRegistration;

bool ASTFunctionNode::isASTFunctionNode() const
{
    return true;
}

AnyValuePtr ASTFunctionNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitFunctionNode(selfFromThis());
}

SExpression ASTFunctionNode::asSExpression() const
{
    SExpressionList argumentsSExpr;
    argumentsSExpr.elements.reserve(arguments.size());
    for(const auto &arg : arguments)
        argumentsSExpr.elements.push_back(arg->asSExpression());

    return SExpressionList{{SExpressionIdentifier{{"function"}},
        SExpressionIdentifier{{programEntityVisibilityToString(visibility)}},
        SExpressionIdentifier{{methodDispatchModeToString(dispatchMode)}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        name ? name->asSExpression() : nullptr,
        argumentsSExpr,
        resultType ? resultType->asSExpression() : nullptr,
        body ? body->asSExpression() : nullptr,
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius