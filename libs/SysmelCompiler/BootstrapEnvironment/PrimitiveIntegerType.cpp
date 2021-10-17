#include "sysmel/BootstrapEnvironment/PrimitiveIntegerType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<PrimitiveIntegerType> PrimitiveIntegerTypeTypeRegistration;
static BootstrapTypeRegistration<UInt8> UInt8TypeRegistration;
static BootstrapTypeRegistration<UInt16> UInt16TypeRegistration;
static BootstrapTypeRegistration<UInt32> UInt32TypeRegistration;
static BootstrapTypeRegistration<UInt64> UInt64TypeRegistration;

static BootstrapTypeRegistration<Int8> Int8TypeRegistration;
static BootstrapTypeRegistration<Int16> Int16TypeRegistration;
static BootstrapTypeRegistration<Int32> Int32TypeRegistration;
static BootstrapTypeRegistration<Int64> Int64TypeRegistration;

bool PrimitiveIntegerType::isPrimitiveIntegerTypeValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius