#include "sysmel/BootstrapEnvironment/FieldMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTFieldVariableNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<FieldMetaBuilder> fieldMetaBuilderTypeRegistration;

ASTNodePtr FieldMetaBuilder::concretizeMetaBuilder()
{
    auto result = basicMakeObject<ASTFieldVariableNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();
    result->visibility = instanceContext->programEntityVisibility;

    result->name = nameNode;
    result->type = typeNode;
    result->alignment = alignmentNode;
    result->initialValue = initialValueNode;
    result->typeInferenceMode = typeInferenceMode;
    result->isMutable = isMutable;
    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius