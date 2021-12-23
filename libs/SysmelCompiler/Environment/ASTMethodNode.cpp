#include "Environment/ASTMethodNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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
        SExpressionIdentifier{{dllLinkageModeToString(dllLinkageMode)}},
        SExpressionIdentifier{{externalLanguageModeToString(externalLanguageMode)}},
        SExpressionIdentifier{{callingConventionToString(callingConvention)}},
        methodFlagsToSExpression(methodFlags),
        name ? name->asSExpression() : nullptr,
        argumentsSExpr,
        resultType ? resultType->asSExpression() : nullptr,
        body ? body->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel