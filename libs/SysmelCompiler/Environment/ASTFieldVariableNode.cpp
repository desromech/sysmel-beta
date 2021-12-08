#include "Environment/ASTFieldVariableNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTFieldVariableNode> ASTFieldVariableNodeTypeRegistration;

bool ASTFieldVariableNode::isASTFieldVariableNode() const
{
    return true;
}

AnyValuePtr ASTFieldVariableNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitFieldVariableNode(selfFromThis());
}

SExpression ASTFieldVariableNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"field"}},
        sourcePosition->asSExpression(),
        SExpressionIdentifier{{isMutable ? "mutable" : "immutable"}},
        SExpressionIdentifier{{typeInferenceModeToString(typeInferenceMode)}},
        name ? name->asSExpression() : nullptr,
        type ? type->asSExpression() : nullptr,
        alignment ? alignment->asSExpression() : nullptr,
        initialValue ? initialValue->asSExpression() : nullptr,
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel