#include "Environment/FieldMetaBuilder.hpp"
#include "Environment/ASTFieldVariableNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel