#include "sysmel/BootstrapEnvironment/PrimitiveTensorType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<PrimitiveTensorType> PrimitiveTensorTypeTypeRegistration;


bool PrimitiveTensorType::isPrimitiveTensorTypeValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius