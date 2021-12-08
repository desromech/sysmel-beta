#include "Environment/PublicMetaBuilder.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<PublicMetaBuilder> publicMetaBuilderTypeRegistration;

ProgramEntityVisibility PublicMetaBuilder::getVisibility() const
{
    return ProgramEntityVisibility::Public;
}

} // End of namespace Environment
} // End of namespace Sysmel