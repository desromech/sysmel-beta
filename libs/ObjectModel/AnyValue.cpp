#include "sysmel/ObjectModel/AnyValue.hpp"
#include "sysmel/ObjectModel/Type.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<AnyValue> anyValueTypeRegistration;

MethodCategories AnyValue::__instanceMethods__()
{
    return MethodCategories{};
}

MethodCategories AnyValue::__typeMethods__()
{
    return MethodCategories{};
}

AnyValue::~AnyValue()
{
}

void AnyValue::initialize()
{
}

bool AnyValue::isType() const
{
    return false;
}

bool AnyValue::isBootstrapType() const
{
    return false;
}

bool AnyValue::isLiteralValue() const
{
    return false;
}

bool AnyValue::isLiteralNumber() const
{
    return false;
}

bool AnyValue::isLiteralInteger() const
{
    return false;
}

bool AnyValue::isLiteralPositiveInteger() const
{
    return false;
}

bool AnyValue::isLiteralNegativeInteger() const
{
    return false;
}

bool AnyValue::isLiteralCharacter() const
{
    return false;
}

bool AnyValue::isLiteralFraction() const
{
    return false;
}

bool AnyValue::isLiteralFloat() const
{
    return false;
}

bool AnyValue::isLiteralString() const
{
    return false;
}

bool AnyValue::isLiteralSymbol() const
{
    return false;
}

bool AnyValue::isLiteralBoolean() const
{
    return false;
}

bool AnyValue::isLiteralBooleanTrue() const
{
    return false;
}

bool AnyValue::isLiteralBooleanFalse() const
{
    return false;
}

bool AnyValue::isLiteralVoid() const
{
    return false;
}

std::string AnyValue::asString() const
{
    return printString();
}

std::string AnyValue::printString() const
{
    return "an AnyValue";
}

AnyValuePtr AnyValue::runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
{
    (void)selector;
    (void)receiver;
    (void)arguments;
    throw CannotEvaluateMessage();
}

AnyValuePtr AnyValue::performWithArguments(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments)
{
    auto type = getType();
    if(!type)
        throw MessageNotUnderstood("Message " + selector->printString() + " is not understood by " + printString() + ".");
    return type->runWithArgumentsIn(selector, arguments, shared_from_this());
}
} // End of namespace ObjectModel
} // End of namespace SysmelMoebius