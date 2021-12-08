#include "Environment/ASTNamespaceNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTNamespaceNode> ASTNamespaceNodeTypeRegistration;

bool ASTNamespaceNode::isASTNamespaceNode() const
{
    return true;
}

AnyValuePtr ASTNamespaceNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitNamespaceNode(selfFromThis());
}

SExpression ASTNamespaceNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"namespace"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        name ? name->asSExpression() : nullptr,
        body ? body->asSExpression() : nullptr,
    }};
}

void ASTNamespaceNode::childrenDo(const ASTIterationBlock &aBlock)
{
    SuperType::childrenDo(aBlock);
    if(name) aBlock(name);
    if(body) aBlock(body);
}

} // End of namespace Environment
} // End of namespace Sysmel