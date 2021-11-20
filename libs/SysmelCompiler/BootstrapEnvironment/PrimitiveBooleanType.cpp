#include "sysmel/BootstrapEnvironment/PrimitiveBooleanType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<PrimitiveBooleanType> PrimitiveBooleanTypeTypeRegistration;
static BootstrapTypeRegistration<Boolean8> Boolean8TypeRegistration;

bool PrimitiveBooleanType::isPrimitiveBooleanTypeValue() const
{
    return true;
}

Boolean8Ptr Boolean8::make(bool value)
{
    auto result = std::make_shared<Boolean8> ();
    result->value = value;
    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius