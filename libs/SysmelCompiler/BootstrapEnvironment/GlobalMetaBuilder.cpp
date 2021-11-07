#include "sysmel/BootstrapEnvironment/GlobalMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTGlobalVariableNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<GlobalMetaBuilder> globalMetaBuilderTypeRegistration;

ASTNodePtr GlobalMetaBuilder::concretizeMetaBuilder()
{
    auto result = std::make_shared<ASTGlobalVariableNode> ();
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