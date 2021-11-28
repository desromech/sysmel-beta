#include "sysmel/BootstrapEnvironment/LiteralBooleanTrue.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius