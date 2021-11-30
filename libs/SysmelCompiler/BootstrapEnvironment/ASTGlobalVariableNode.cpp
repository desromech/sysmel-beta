#include "sysmel/BootstrapEnvironment/ASTGlobalVariableNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/ASTVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTGlobalVariableNode> ASTGlobalVariableNodeTypeRegistration;

bool ASTGlobalVariableNode::isASTGlobalVariableNode() const
{
    return true;
}

AnyValuePtr ASTGlobalVariableNode::accept(const ASTVisitorPtr &visitor)
{
    return visitor->visitGlobalVariableNode(selfFromThis());
}

SExpression ASTGlobalVariableNode::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"globalVariable"}},
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius