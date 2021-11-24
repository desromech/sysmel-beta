#include "sysmel/BootstrapEnvironment/ASTBreakNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTBreakNode> ASTBreakNodeTypeRegistration;

bool ASTBreakNode::isASTBreakNode() const
{
    return true;
}

AnyValuePtr ASTBreakNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitBreakNode(shared_from_this());
}

SExpression ASTBreakNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"break"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius