#include "sysmel/BootstrapEnvironment/ASTClassNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius