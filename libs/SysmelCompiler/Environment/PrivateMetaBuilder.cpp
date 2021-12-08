#include "Environment/PrivateMetaBuilder.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<PrivateMetaBuilder> privateMetaBuilderTypeRegistration;

ProgramEntityVisibility PrivateMetaBuilder::getVisibility() const
{
    return ProgramEntityVisibility::Private;
}

} // End of namespace Environment
} // End of namespace Sysmel