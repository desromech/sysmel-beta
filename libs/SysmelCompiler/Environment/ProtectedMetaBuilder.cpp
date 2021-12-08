#include "Environment/ProtectedMetaBuilder.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ProtectedMetaBuilder> protectedMetaBuilderTypeRegistration;

ProgramEntityVisibility ProtectedMetaBuilder::getVisibility() const
{
    return ProgramEntityVisibility::Protected;
}

} // End of namespace Environment
} // End of namespace Sysmel