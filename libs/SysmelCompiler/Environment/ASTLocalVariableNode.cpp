#include "Environment/ASTLocalVariableNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTLocalVariableNode> ASTLocalVariableNodeTypeRegistration;

bool ASTLocalVariableNode::isASTLocalVariableNode() const
{
    return true;
}

AnyValuePtr ASTLocalVariableNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitLocalVariableNode(selfFromThis());
}

SExpression ASTLocalVariableNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"localVariable"}},
        SExpressionIdentifier{{isMutable ? "mutable" : "immutable"}},
        SExpressionIdentifier{{typeInferenceModeToString(typeInferenceMode)}},
        analyzedType ? analyzedType->asSExpression() : nullptr,
        sourcePosition->asSExpression(),
        name ? name->asSExpression() : nullptr,
        type ? type->asSExpression() : nullptr,
        alignment ? alignment->asSExpression() : nullptr,
        initialValue ? initialValue->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel