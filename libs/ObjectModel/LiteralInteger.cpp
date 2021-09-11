#include "sysmel/ObjectModel/LiteralInteger.hpp"
#include "sysmel/ObjectModel/LiteralNegativeInteger.hpp"
#include "sysmel/ObjectModel/LiteralPositiveInteger.hpp"
#include "sysmel/ObjectModel/LiteralCharacter.hpp"

#include "sysmel/ObjectModel/LiteralSymbol.hpp"
#include "sysmel/ObjectModel/LiteralFraction.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include <algorithm>
#include <limits>

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<LiteralInteger> literalIntegerTypeRegistration;

TypePtr WrapperTypeFor<LargeInteger>::apply()
{
    return LiteralInteger::__staticType__();
}

MethodCategories LiteralInteger::__instanceMethods__()
{
    return MethodCategories{
        {"arithmetic", {
            makeMethodBinding<LargeInteger (LargeInteger)> ("negated", +[](const LargeInteger &value) {
                return -value;
            }),

            // Addition
            makeMethodBinding<LargeInteger (LargeInteger, LargeInteger)> ("+", +[](const LargeInteger &a, const LargeInteger &b) {
                return a + b;
            }),

            makeMethodBinding<double (LargeInteger, double)> ("+", +[](const LargeInteger &a, double b) {
                return a.asDouble() + b;
            }),

            // Subtraction
            makeMethodBinding<LargeInteger (LargeInteger, LargeInteger)> ("-", +[](const LargeInteger &a, const LargeInteger &b) {
                return a - b;
            }),

            makeMethodBinding<double (LargeInteger, double)> ("-", +[](const LargeInteger &a, double b) {
                return a.asDouble() - b;
            }),

            // Multiplication
            makeMethodBinding<LargeInteger (LargeInteger, LargeInteger)> ("*", +[](const LargeInteger &a, const LargeInteger &b) {
                return a * b;
            }),

            makeMethodBinding<double (LargeInteger, double)> ("*", +[](const LargeInteger &a, double b) {
                return a.asDouble() * b;
            }),

            // Division
            makeMethodBinding<Fraction (LargeInteger, LargeInteger)> ("/", +[](const LargeInteger &a, const LargeInteger &b) {
                if(b.isZero())
                    throw DivisionByZeroError();

                return Fraction{a, b}.reduced();
            }),

            makeMethodBinding<double (LargeInteger, double)> ("/", +[](const LargeInteger &a, double b) {
                return a.asDouble() / b;
            }),

            // Integer division
            makeMethodBinding<LargeInteger (LargeInteger, LargeInteger)> ("//", +[](const LargeInteger &a, const LargeInteger &b) {
                if(b.isZero())
                    throw DivisionByZeroError();
                return a / b;
            }),

            // Remainder
            makeMethodBinding<LargeInteger (LargeInteger, LargeInteger)> ("%", +[](const LargeInteger &a, const LargeInteger &b) {
                if(b.isZero())
                    throw DivisionByZeroError();

                return a % b;
            }),

            makeMethodBinding<LargeInteger (LargeInteger, LargeInteger)> ("\\\\", +[](const LargeInteger &a, const LargeInteger &b) {
                if(b.isZero())
                    throw DivisionByZeroError();

                return a % b;
            }),
        }},

        {"math functions", {
            makeMethodBinding<LargeInteger (LargeInteger)> ("factorial", +[](const LargeInteger &value) {
                return value.factorial();
            })
        }}
    };
}

LiteralIntegerPtr LiteralInteger::makeFor(const LargeInteger &value)
{
    LiteralIntegerPtr result;
    if(value.isNegative())
        result = std::make_shared<LiteralNegativeInteger> ();
    else
        result = std::make_shared<LiteralPositiveInteger> ();
    result->value = value;
    return result;
}

LiteralIntegerPtr LiteralInteger::makeFor(LargeInteger &&value)
{
    LiteralIntegerPtr result;
    if(value.isNegative())
        result = std::make_shared<LiteralNegativeInteger> ();
    else
        result = std::make_shared<LiteralPositiveInteger> ();
    result->value = std::move(value);
    return result;
}

LiteralIntegerPtr LiteralInteger::makeForCharacter(char32_t value)
{
    LiteralIntegerPtr result = std::make_shared<LiteralCharacter> ();
    result->value = LargeInteger{value};
    return result;
}

bool LiteralInteger::isLiteralInteger() const
{
    return true;
}

std::string LiteralInteger::printString() const
{
    return value.asString();
}

uint8_t LiteralInteger::unwrapAsUInt8() const
{
    if(value.isNegative() || value > LargeInteger{std::numeric_limits<uint8_t>::max()})
        throw CannotUnwrap();
    return uint8_t(value.wordAt(0));
}

int8_t LiteralInteger::unwrapAsInt8() const
{
    if(value < LargeInteger{std::numeric_limits<int8_t>::min()} || value > LargeInteger{std::numeric_limits<int8_t>::max()})
        throw CannotUnwrap();
    return value.isNegative() ? int8_t(-value.wordAt(0)) : int8_t(value.wordAt(0));
}

uint16_t LiteralInteger::unwrapAsUInt16() const
{
    if(value.isNegative() || value > LargeInteger{std::numeric_limits<uint16_t>::max()})
        throw CannotUnwrap();
    return uint16_t(value.wordAt(0));
}

int16_t LiteralInteger::unwrapAsInt16() const
{
    if(value < LargeInteger{std::numeric_limits<int16_t>::min()} || value > LargeInteger{std::numeric_limits<int16_t>::max()})
        throw CannotUnwrap();
    return value.isNegative() ? int16_t(-value.wordAt(0)) : int16_t(value.wordAt(0));
}

uint32_t LiteralInteger::unwrapAsUInt32() const
{
    if(value.isNegative() || value > LargeInteger{std::numeric_limits<uint32_t>::max()})
        throw CannotUnwrap();
    return value.wordAt(0);
}

int32_t LiteralInteger::unwrapAsInt32() const
{
    if(value < LargeInteger{std::numeric_limits<int32_t>::min()} || value > LargeInteger{std::numeric_limits<int32_t>::max()})
        throw CannotUnwrap();
    return value.isNegative() ? int32_t(-value.wordAt(0)) : int32_t(value.wordAt(0));
}

uint64_t LiteralInteger::unwrapAsUInt64() const
{
    if(value.isNegative() || value > LargeInteger{std::numeric_limits<uint64_t>::max()})
        throw CannotUnwrap();
    return value.wordAt(0) | uint64_t(value.wordAt(1)) << 32;
}

int64_t LiteralInteger::unwrapAsInt64() const
{
    if(value < LargeInteger{std::numeric_limits<int64_t>::min()} || value > LargeInteger{std::numeric_limits<int64_t>::max()})
        throw CannotUnwrap();

    auto magnitude = int64_t(value.wordAt(0) | uint64_t(value.wordAt(1)) << 32);
    return value.isNegative() ? -magnitude : magnitude;
}

LargeInteger LiteralInteger::unwrapAsLargeInteger() const
{
    return value;
}

char LiteralInteger::unwrapAsChar8() const
{
    if(value.isNegative() || value > LargeInteger{std::numeric_limits<char>::max()})
        throw CannotUnwrap();
    return char(value.wordAt(0));
}

char16_t LiteralInteger::unwrapAsChar16() const
{
    if(value.isNegative() || value > LargeInteger{std::numeric_limits<char16_t>::max()})
        throw CannotUnwrap();
    return char16_t(value.wordAt(0));
}

char32_t LiteralInteger::unwrapAsChar32() const
{
    if(value.isNegative() || value > LargeInteger{std::numeric_limits<char32_t>::max()})
        throw CannotUnwrap();
    return char32_t(value.wordAt(0));
}

Fraction LiteralInteger::unwrapAsFraction() const
{
    return Fraction{value, LargeInteger{1}};
}

float LiteralInteger::unwrapAsFloat32() const
{

    throw CannotUnwrap();
}

double LiteralInteger::unwrapAsFloat64() const
{
    return value.asDouble();
}

double LiteralInteger::asFloat() const
{
    return value.asDouble();
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius