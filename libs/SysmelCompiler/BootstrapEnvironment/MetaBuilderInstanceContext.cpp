#include "sysmel/BootstrapEnvironment/MetaBuilderInstanceContext.hpp"
#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<MetaBuilderInstanceContext> metaBuilderTypeRegistration;

ASTSourcePositionPtr MetaBuilderInstanceContext::concreteSourcePosition()
{
    return instanceNode->sourcePosition;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius
