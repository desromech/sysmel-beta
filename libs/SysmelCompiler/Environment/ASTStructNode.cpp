#include "Environment/ASTStructNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTStructNode> ASTStructNodeRegistration;

bool ASTStructNode::isASTStructNode() const
{
    return true;
}

AnyValuePtr ASTStructNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitStructNode(selfFromThis());
}

SExpression ASTStructNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"struct"}},
        sourcePosition->asSExpression(),
        SExpressionIdentifier{{programEntityVisibilityToString(visibility)}},
        SExpressionIdentifier{{dllLinkageModeToString(dllLinkageMode)}},
        SExpressionIdentifier{{externalLanguageModeToString(externalLanguageMode)}},
        isPacked,
        name ? name->asSExpression() : nullptr,
        body ? body->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel