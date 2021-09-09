#include "sysmel/ObjectModel/AnyValue.hpp"
#include "sysmel/ObjectModel/Type.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<AnyValue> anyValueTypeRegistration;

MethodCategories AnyValue::__instanceMethods__()
{    return MethodCategories{
        {"printing", {
            makeMethodBinding("printString", &AnyValue::printString),
            makeMethodBinding("asString", &AnyValue::asString),
        }}
    };
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

bool AnyValue::isProgramEntity() const
{
    return false;
}


bool AnyValue::isType() const
{
    return false;
}

bool AnyValue::isBootstrapType() const
{
    return false;
}

bool AnyValue::isMethod() const
{
    return false;
}

bool AnyValue::isSpecificMethod() const
{
    return false;
}

bool AnyValue::isBootstrapMethod() const
{
    return false;
}

bool AnyValue::isPatternMatchingMethod() const
{
    return false;
}

bool AnyValue::isTemplateMethod() const
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

uint8_t AnyValue::unwrapAsUInt8() const
{
    throw CannotUnwrap();
}

int8_t AnyValue::unwrapAsInt8() const
{
    throw CannotUnwrap();
}

uint16_t AnyValue::unwrapAsUInt16() const
{
    throw CannotUnwrap();
}

int16_t AnyValue::unwrapAsInt16() const
{
    throw CannotUnwrap();
}

uint32_t AnyValue::unwrapAsUInt32() const
{
    throw CannotUnwrap();
}

int32_t AnyValue::unwrapAsInt32() const
{
    throw CannotUnwrap();
}

uint64_t AnyValue::unwrapAsUInt64() const
{
    throw CannotUnwrap();
}

int64_t AnyValue::unwrapAsInt64() const
{
    throw CannotUnwrap();
}

LargeInteger AnyValue::unwrapAsLargeInteger() const
{
    throw CannotUnwrap();
}

Fraction AnyValue::unwrapAsFraction() const
{
    throw CannotUnwrap();
}

float AnyValue::unwrapAsFloat32() const
{
    throw CannotUnwrap();
}

double AnyValue::unwrapAsFloat64() const
{
    throw CannotUnwrap();
}

char AnyValue::unwrapAsChar8() const
{
    throw CannotUnwrap();
}

char16_t AnyValue::unwrapAsChar16() const
{
    throw CannotUnwrap();
}

char32_t AnyValue::unwrapAsChar32() const
{
    throw CannotUnwrap();
}

std::string AnyValue::unwrapAsString() const
{
    throw CannotUnwrap();
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