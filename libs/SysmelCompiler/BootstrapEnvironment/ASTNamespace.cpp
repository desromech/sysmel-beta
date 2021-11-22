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
    return visitor->visitNamespaceNode(shared_from_this());
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius