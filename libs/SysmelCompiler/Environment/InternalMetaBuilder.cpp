#include "Environment/InternalMetaBuilder.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<InternalMetaBuilder> internalMetaBuilderTypeRegistration;

ProgramEntityVisibility InternalMetaBuilder::getVisibility() const
{
    return ProgramEntityVisibility::Internal;
}

} // End of namespace Environment
} // End of namespace Sysmel