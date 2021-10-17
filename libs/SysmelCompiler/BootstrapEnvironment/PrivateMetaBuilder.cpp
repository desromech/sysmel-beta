#include "sysmel/BootstrapEnvironment/PrivateMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<PrivateMetaBuilder> privateMetaBuilderTypeRegistration;

ProgramEntityVisibility PrivateMetaBuilder::getVisibility() const
{
    return ProgramEntityVisibility::Private;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius