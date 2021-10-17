#include "sysmel/BootstrapEnvironment/PublicMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<PublicMetaBuilder> publicMetaBuilderTypeRegistration;

ProgramEntityVisibility PublicMetaBuilder::getVisibility() const
{
    return ProgramEntityVisibility::Public;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius