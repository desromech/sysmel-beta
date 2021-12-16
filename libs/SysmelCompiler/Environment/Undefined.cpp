#include "Environment/Undefined.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
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
    return true;
}

AnyValuePtr Undefined::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitUndefined(selfFromThis());
}

std::string Undefined::printString() const
{
    return "nil";
}

SExpression Undefined::asSExpression() const
{
    return nullptr;
}

} // End of namespace Environment
} // End of namespace Sysmel