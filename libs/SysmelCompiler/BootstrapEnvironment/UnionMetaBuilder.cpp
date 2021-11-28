#include "sysmel/BootstrapEnvironment/UnionMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTUnionNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius