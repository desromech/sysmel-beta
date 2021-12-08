#include "Environment/ASTFunctionNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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
        methodFlagsToSExpression(methodFlags),
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        name ? name->asSExpression() : nullptr,
        argumentsSExpr,
        resultType ? resultType->asSExpression() : nullptr,
        body ? body->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel