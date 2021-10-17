#include "sysmel/BootstrapEnvironment/ProtectedMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ProtectedMetaBuilder> protectedMetaBuilderTypeRegistration;

ProgramEntityVisibility ProtectedMetaBuilder::getVisibility() const
{
    return ProgramEntityVisibility::Protected;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius