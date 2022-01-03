#include "Environment/ASTSlotAccessNode.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTSlotAccessNode> ASTSlotAccessNodeRegistration;

bool ASTSlotAccessNode::isASTSlotAccessNode() const
{
    return true;
}

AnyValuePtr ASTSlotAccessNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitSlotAccessNode(selfFromThis());
}

SExpression ASTSlotAccessNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"slotAccess"}},
        sourcePosition->asSExpression(),
        analyzedType ? analyzedType->asSExpression() : nullptr,
        aggregate->asSExpression(),
        LargeInteger{slotIndex},
        LargeInteger{slotOffset},
        slotReferenceType
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel