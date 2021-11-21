#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<BootstrapMethodBase> bootstrapMethodBaseTypeRegistration;

bool BootstrapMethodBase::isBootstrapMethod() const
{
    return true;
}

bool BootstrapMethodBase::isCompileTimeEvaluableMethod() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius