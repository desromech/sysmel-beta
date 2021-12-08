#include "Environment/ASTLocalImmutableAccessNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTLocalImmutableAccessNode> ASTLocalImmutableAccessNodeRegistration;

bool ASTLocalImmutableAccessNode::isASTLocalImmutableAccessNode() const
{
    return true;
}

AnyValuePtr ASTLocalImmutableAccessNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitLocalImmutableAccessNode(selfFromThis());
}

SExpression ASTLocalImmutableAccessNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"localImmutableAccessNode"}},
        analyzedType ? analyzedType->asSExpression() : nullptr,
        bindingName->asSExpression(),
        sourcePosition->asSExpression(),
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel