#include "Environment/ASTClassNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTClassNode> ASTClassNodeRegistration;

bool ASTClassNode::isASTClassNode() const
{
    return true;
}

AnyValuePtr ASTClassNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitClassNode(selfFromThis());
}

SExpression ASTClassNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"class"}},
        sourcePosition->asSExpression(),
        SExpressionIdentifier{{programEntityVisibilityToString(visibility)}},
        SExpressionIdentifier{{dllLinkageModeToString(dllLinkageMode)}},
        SExpressionIdentifier{{externalLanguageModeToString(externalLanguageMode)}},
        name ? name->asSExpression() : nullptr,
        superclass ? superclass->asSExpression() : nullptr,
        body ? body->asSExpression() : nullptr,
    }};
}

void ASTClassNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(superclass) aBlock(superclass);
}

} // End of namespace Environment
} // End of namespace Sysmel