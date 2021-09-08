#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<BootstrapMethodBase> bootstrapMethodBaseTypeRegistration;

bool BootstrapMethodBase::isBootstrapMethod() const
{
    return true;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius