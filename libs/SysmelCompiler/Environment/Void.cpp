#include "Environment/Void.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<Void> voidRegistration;

TypePtr WrapperTypeFor<void>::apply()
{
    return Void::__staticType__();
}

TypePtr Type::getVoidType()
{
    return Void::__staticType__();
}

AnyValuePtr getVoidConstant()
{
    return Void::uniqueInstance();
}

VoidPtr Void::uniqueInstance()
{
    static auto singleton = basicMakeGlobalSingletonObject<Void> ();
    return singleton;
}

bool Void::isVoid() const
{
    return true;
}

std::string Void::printString() const
{
    return "void";
}

SExpression Void::asSExpression() const
{
    return SExpressionVoid{};
}

} // End of namespace Environment
} // End of namespace Sysmel