#include "sysmel/BootstrapEnvironment/PointerLikeType.hpp"
#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<PointerLikeTypeValue> PointerLikeTypeValueTypeRegistration;

bool PointerLikeType::isPointerLikeType() const
{
    return true;
}

bool PointerLikeType::supportsDynamicCompileTimeMessageSend() const
{
    return false;
}

bool PointerLikeType::isNullableType() const
{
    return false;
}

bool PointerLikeType::isImmutableType() const
{
    return true;
}

bool PointerLikeType::hasTrivialInitialization() const
{
    return true;
}

bool PointerLikeType::hasTrivialFinalization() const
{
    return true;
}

bool PointerLikeType::hasTrivialCopyingFrom() const
{
    return true;
}

bool PointerLikeType::hasTrivialMovingFrom() const
{
    return true;
}

bool PointerLikeType::hasGenericAddressSpace() const
{
    return addressSpace == internSymbol("generic");
}

PointerLikeTypeValuePtr PointerLikeType::makeWithValue(const AnyValuePtr &value)
{
    SysmelSelfSubclassResponsibility();
}

bool PointerLikeTypeValue::isPointerLikeTypeValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius