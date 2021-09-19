#include "sysmel/BootstrapEnvironment/LiteralBooleanFalse.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

SExpression LiteralBooleanFalse::asSExpression() const
{
    return false;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius