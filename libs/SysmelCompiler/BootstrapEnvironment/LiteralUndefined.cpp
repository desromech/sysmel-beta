#include "sysmel/BootstrapEnvironment/LiteralUndefined.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<LiteralUndefined> literalUndefinedRegistration;

AnyValuePtr getNilConstant()
{
    return LiteralUndefined::uniqueInstance();
}

LiteralUndefinedPtr LiteralUndefined::uniqueInstance()
{
    auto singleton = std::make_shared<LiteralUndefined> ();
    return singleton;
}

bool LiteralUndefined::isLiteralUndefined() const
{
    return true;
}

std::string LiteralUndefined::printString() const
{
    return "nil";
}

SExpression LiteralUndefined::asSExpression() const
{
    return nullptr;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius