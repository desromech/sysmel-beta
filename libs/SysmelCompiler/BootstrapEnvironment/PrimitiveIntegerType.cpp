#include "sysmel/BootstrapEnvironment/PrimitiveIntegerType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "IntrinsicPrimitiveIntegerMethods.hpp"

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

MethodCategories UInt8::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt8>::instanceMethods();
}

MethodCategories UInt16::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt16>::instanceMethods();
}

MethodCategories UInt32::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt32>::instanceMethods();
}

MethodCategories UInt64::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt64>::instanceMethods();
}

MethodCategories Int8::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt8>::instanceMethods();
}

MethodCategories Int16::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt16>::instanceMethods();
}

MethodCategories Int32::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt32>::instanceMethods();
}

MethodCategories Int64::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<UInt64>::instanceMethods();
}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius