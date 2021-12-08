#include "Environment/StructMetaBuilder.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTStructNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<StructMetaBuilder> structMetaBuilderTypeRegistration;

ASTNodePtr StructMetaBuilder::concretizeMetaBuilder()
{
    auto result = basicMakeObject<ASTStructNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();
    result->visibility = instanceContext->programEntityVisibility;

    result->name = nameNode;
    result->body = bodyNode;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel