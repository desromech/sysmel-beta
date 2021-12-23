#include "Environment/ASTGlobalVariableNode.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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
        SExpressionIdentifier{{dllLinkageModeToString(dllLinkageMode)}},
        SExpressionIdentifier{{externalLanguageModeToString(externalLanguageMode)}},
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