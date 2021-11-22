#include "sysmel/BootstrapEnvironment/PointerLikeType.hpp"
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

bool PointerLikeTypeValue::isPointerLikeTypeValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius