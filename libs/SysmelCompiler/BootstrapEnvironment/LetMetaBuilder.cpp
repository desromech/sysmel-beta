#include "sysmel/BootstrapEnvironment/LetMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTLocalVariableNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<LetMetaBuilder> letMetaBuilderTypeRegistration;

ASTNodePtr LetMetaBuilder::concretizeMetaBuilder()
{
    auto result = std::make_shared<ASTLocalVariableNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();

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