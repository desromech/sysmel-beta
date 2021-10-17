#include "sysmel/BootstrapEnvironment/PrimitiveCharacterType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius