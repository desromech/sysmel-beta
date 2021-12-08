#include "Environment/LiteralBooleanFalse.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<LiteralBooleanFalse> literalBooleanFalseRegistration;

AnyValuePtr getFalseConstant()
{
    return LiteralBooleanFalse::uniqueInstance();
}

LiteralBooleanFalsePtr LiteralBooleanFalse::uniqueInstance()
{
    static auto singleton = basicMakeGlobalSingletonObject<LiteralBooleanFalse> ();
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

std::string LiteralBooleanFalse::printString() const
{
    return "false";
}

SExpression LiteralBooleanFalse::asSExpression() const
{
    return false;
}

} // End of namespace Environment
} // End of namespace Sysmel