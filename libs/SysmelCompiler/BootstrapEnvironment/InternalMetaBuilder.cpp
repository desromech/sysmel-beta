#include "sysmel/BootstrapEnvironment/InternalMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<InternalMetaBuilder> internalMetaBuilderTypeRegistration;

ProgramEntityVisibility InternalMetaBuilder::getVisibility() const
{
    return ProgramEntityVisibility::Internal;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius