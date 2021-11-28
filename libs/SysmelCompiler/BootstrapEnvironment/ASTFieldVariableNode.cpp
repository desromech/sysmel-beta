#include "sysmel/BootstrapEnvironment/ASTFieldVariableNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius