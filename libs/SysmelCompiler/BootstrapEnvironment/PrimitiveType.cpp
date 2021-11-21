#include "sysmel/BootstrapEnvironment/PrimitiveType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<PrimitiveType> PrimitiveTypeTypeRegistration;

bool PrimitiveType::isPrimitiveTypeValue() const
{
    return true;
}

bool PrimitiveType::isPureCompileTimeLiteralValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius