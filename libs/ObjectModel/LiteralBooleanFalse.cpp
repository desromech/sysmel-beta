#include "sysmel/ObjectModel/LiteralBooleanFalse.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace ObjectModel
{
static BootstrapTypeRegistration<LiteralBooleanFalse> literalBooleanFalseRegistration;

AnyValuePtr getFalseConstant()
{
    return LiteralBooleanFalse::uniqueInstance();
}

LiteralBooleanFalsePtr LiteralBooleanFalse::uniqueInstance()
{
    auto singleton = std::make_shared<LiteralBooleanFalse> ();
    return singleton;
}

bool LiteralBooleanFalse::isLiteralBooleanFalse() const
{
    return true;
}

bool LiteralBooleanFalse::unwrapAsBoolean() const
{
    return false;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius