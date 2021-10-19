#include "sysmel/BootstrapEnvironment/StructMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTStructNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<StructMetaBuilder> structMetaBuilderTypeRegistration;

ASTNodePtr StructMetaBuilder::concretizeMetaBuilder()
{
    auto result = std::make_shared<ASTStructNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();
    result->visibility = instanceContext->programEntityVisibility;

    result->name = nameNode;
    result->body = bodyNode;
    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius