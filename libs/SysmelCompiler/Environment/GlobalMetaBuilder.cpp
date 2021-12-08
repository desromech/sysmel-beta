#include "Environment/GlobalMetaBuilder.hpp"
#include "Environment/ASTGlobalVariableNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<GlobalMetaBuilder> globalMetaBuilderTypeRegistration;

ASTNodePtr GlobalMetaBuilder::concretizeMetaBuilder()
{
    auto result = basicMakeObject<ASTGlobalVariableNode> ();
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