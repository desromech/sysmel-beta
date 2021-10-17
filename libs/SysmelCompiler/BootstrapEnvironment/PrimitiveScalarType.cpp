#include "sysmel/BootstrapEnvironment/PrimitiveScalarType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<PrimitiveScalarType> PrimitiveScalarTypeTypeRegistration;


bool PrimitiveScalarType::isPrimitiveScalarTypeValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius