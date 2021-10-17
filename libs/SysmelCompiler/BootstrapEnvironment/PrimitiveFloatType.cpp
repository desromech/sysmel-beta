#include "sysmel/BootstrapEnvironment/PrimitiveFloatType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<PrimitiveFloatType> PrimitiveFloatTypeTypeRegistration;
static BootstrapTypeRegistration<Float16> Float16TypeRegistration;
static BootstrapTypeRegistration<Float32> Float32TypeRegistration;
static BootstrapTypeRegistration<Float64> Float64TypeRegistration;

bool PrimitiveFloatType::isPrimitiveFloatTypeValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius