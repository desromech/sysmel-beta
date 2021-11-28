#include "sysmel/BootstrapEnvironment/Undefined.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<Undefined> literalUndefinedRegistration;

TypePtr Type::getUndefinedType()
{
    return Undefined::__staticType__();
}

AnyValuePtr getNilConstant()
{
    return Undefined::uniqueInstance();
}

UndefinedPtr Undefined::uniqueInstance()
{
    static auto singleton = basicMakeGlobalSingletonObject<Undefined> ();
    return singleton;
}

bool Undefined::isUndefined() const
{
    return true;
}

bool Undefined::isAnonymousNameSymbol() const
{
    return false;
}

std::string Undefined::printString() const
{
    return "nil";
}

SExpression Undefined::asSExpression() const
{
    return nullptr;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius