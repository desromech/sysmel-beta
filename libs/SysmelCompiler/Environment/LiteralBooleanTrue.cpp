#include "Environment/LiteralBooleanTrue.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<LiteralBooleanTrue> literalBooleanTrueRegistration;

AnyValuePtr getTrueConstant()
{
    return LiteralBooleanTrue::uniqueInstance();
}

LiteralBooleanTruePtr LiteralBooleanTrue::uniqueInstance()
{
    static auto singleton = basicMakeGlobalSingletonObject<LiteralBooleanTrue> ();
    return singleton;
}

bool LiteralBooleanTrue::isLiteralBooleanTrue() const
{
    return true;
}

bool LiteralBooleanTrue::unwrapAsBoolean() const
{
    return true;
}

std::string LiteralBooleanTrue::printString() const
{
    return "true";
}

SExpression LiteralBooleanTrue::asSExpression() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel