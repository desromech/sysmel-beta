#include "Environment/PointerLikeType.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

bool PointerLikeType::isImmutableType()
{
    return true;
}

bool PointerLikeType::hasTrivialInitialization()
{
    return true;
}

bool PointerLikeType::hasTrivialFinalization()
{
    return true;
}

bool PointerLikeType::hasTrivialCopyingFrom()
{
    return true;
}

bool PointerLikeType::hasTrivialMovingFrom()
{
    return true;
}

bool PointerLikeType::hasGenericAddressSpace() const
{
    return addressSpace == internSymbol("generic");
}

PointerLikeTypeValuePtr PointerLikeType::makeWithValue(const AnyValuePtr &value)
{
    (void)value;
    SysmelSelfSubclassResponsibility();
}

bool PointerLikeTypeValue::isPointerLikeTypeValue() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel