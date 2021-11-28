#include "sysmel/BootstrapEnvironment/ASTNamespaceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius