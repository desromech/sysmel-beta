#include "Environment/UnionMetaBuilder.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTUnionNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<UnionMetaBuilder> unionMetaBuilderTypeRegistration;

ASTNodePtr UnionMetaBuilder::concretizeMetaBuilder()
{
    auto result = basicMakeObject<ASTUnionNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();
    result->visibility = instanceContext->programEntityVisibility;

    result->name = nameNode;
    result->body = bodyNode;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel