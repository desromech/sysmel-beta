#include "sysmel/BootstrapEnvironment/PrimitiveNumberType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<PrimitiveNumberType> PrimitiveNumberTypeTypeRegistration;


bool PrimitiveNumberType::isPrimitiveNumberTypeValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius