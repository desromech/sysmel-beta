#include "Environment/LetMetaBuilder.hpp"
#include "Environment/ASTLocalVariableNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<LetMetaBuilder> letMetaBuilderTypeRegistration;

ASTNodePtr LetMetaBuilder::concretizeMetaBuilder()
{
    auto result = basicMakeObject<ASTLocalVariableNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();

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