#include "sysmel/BootstrapEnvironment/ASTLocalVariableNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTLocalVariableNode> ASTLocalVariableNodeTypeRegistration;

bool ASTLocalVariableNode::isASTLocalVariableNode() const
{
    return true;
}

AnyValuePtr ASTLocalVariableNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitLocalVariableNode(shared_from_this());
}

SExpression ASTLocalVariableNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"localVariable"}},
        sourcePosition->asSExpression(),
        SExpressionIdentifier{{isMutable ? "mutable" : "immutable"}},
        SExpressionIdentifier{{typeInferenceModeToString(typeInferenceMode)}},
        name ? name->asSExpression() : nullptr,
        type ? type->asSExpression() : nullptr,
        alignment ? alignment->asSExpression() : nullptr,
        initialValue ? initialValue->asSExpression() : nullptr,
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius