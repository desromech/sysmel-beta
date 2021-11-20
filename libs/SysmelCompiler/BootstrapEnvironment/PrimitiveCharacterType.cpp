#include "sysmel/BootstrapEnvironment/PrimitiveCharacterType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "IntrinsicPrimitiveIntegerMethods.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<PrimitiveCharacterType> PrimitiveCharacterTypeRegistration;
static BootstrapTypeRegistration<Char8> Char8TypeRegistration;
static BootstrapTypeRegistration<Char16> Char16TypeRegistration;
static BootstrapTypeRegistration<Char32> Char32TypeRegistration;

bool PrimitiveCharacterType::isPrimitiveCharacterTypeValue() const
{
    return true;
}

MethodCategories Char8::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Char8>::instanceMethods();
}

MethodCategories Char16::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Char16>::instanceMethods();
}

MethodCategories Char32::__instanceMethods__()
{
    return IntrinsicPrimitiveIntegerMethods<Char32>::instanceMethods();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius