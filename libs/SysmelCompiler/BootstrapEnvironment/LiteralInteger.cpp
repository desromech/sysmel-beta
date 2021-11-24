#include "sysmel/BootstrapEnvironment/LiteralInteger.hpp"
#include "sysmel/BootstrapEnvironment/LiteralNegativeInteger.hpp"
#include "sysmel/BootstrapEnvironment/LiteralPositiveInteger.hpp"
#include "sysmel/BootstrapEnvironment/LiteralCharacter.hpp"

#include "sysmel/BootstrapEnvironment/LiteralSymbol.hpp"
#include "sysmel/BootstrapEnvironment/LiteralFraction.hpp"
#include "sysmel/BootstrapEnvironment/DivisionByZeroError.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include <algorithm>
#include <limits>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<LiteralInteger> literalIntegerTypeRegistration;

TypePtr WrapperTypeFor<LargeInteger>::apply()
{
    return LiteralInteger::__staticType__();
}

MethodCategories LiteralInteger::__instanceMethods__()
{
    return MethodCategories{
        {"comparisons", {
            // =
            makeMethodBinding<bool (LargeInteger, LargeInteger)> ("=", +[](const LargeInteger &a, const LargeInteger &b) {
                return a == b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, Fraction)> ("=", +[](const LargeInteger &a, const Fraction &b) {
                return Fraction{a} == b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, double)> ("=", +[](const LargeInteger &a, double b) {
                return a.asDouble() == b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, AnyValuePtr)> ("=", +[](const LargeInteger&, const AnyValuePtr &) {
                return false;
            }, MethodFlags::Pure),

            // ~=
            makeMethodBinding<bool (LargeInteger, LargeInteger)> ("~=", +[](const LargeInteger &a, const LargeInteger &b) {
                return a != b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, Fraction)> ("~=", +[](const LargeInteger &a, const Fraction &b) {
                return Fraction{a} != b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, double)> ("~=", +[](const LargeInteger &a, double b) {
                return a.asDouble() != b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, AnyValuePtr)> ("~=", +[](const LargeInteger&, const AnyValuePtr &) {
                return true;
            }, MethodFlags::Pure),

            // <
            makeMethodBinding<bool (LargeInteger, LargeInteger)> ("<", +[](const LargeInteger &a, const LargeInteger &b) {
                return a < b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, Fraction)> ("<", +[](const LargeInteger &a, const Fraction &b) {
                return Fraction{a} < b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, double)> ("<", +[](const LargeInteger &a, double b) {
                return a.asDouble() < b;
            }, MethodFlags::Pure),

            // <=
            makeMethodBinding<bool (LargeInteger, LargeInteger)> ("<=", +[](const LargeInteger &a, const LargeInteger &b) {
                return a <= b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, Fraction)> ("<=", +[](const LargeInteger &a, const Fraction &b) {
                return Fraction{a} <= b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, double)> ("<=", +[](const LargeInteger &a, double b) {
                return a.asDouble() <= b;
            }, MethodFlags::Pure),

            // >
            makeMethodBinding<bool (LargeInteger, LargeInteger)> (">", +[](const LargeInteger &a, const LargeInteger &b) {
                return a > b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, Fraction)> (">", +[](const LargeInteger &a, const Fraction &b) {
                return Fraction{a} > b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, double)> (">", +[](const LargeInteger &a, double b) {
                return a.asDouble() > b;
            }, MethodFlags::Pure),

            // >=
            makeMethodBinding<bool (LargeInteger, LargeInteger)> (">=", +[](const LargeInteger &a, const LargeInteger &b) {
                return a >= b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, Fraction)> (">=", +[](const LargeInteger &a, const Fraction &b) {
                return Fraction{a} >= b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (LargeInteger, double)> (">=", +[](const LargeInteger &a, double b) {
                return a.asDouble() >= b;
            }, MethodFlags::Pure),
        }},
        {"arithmetic", {
            // Negation
            makeMethodBinding<LargeInteger (LargeInteger)> ("negated", +[](const LargeInteger &value) {
                return -value;
            }, MethodFlags::Pure),
            makeMethodBinding<LargeInteger (LargeInteger)> ("pre--", +[](const LargeInteger &value) {
                return -value;
            }, MethodFlags::Pure),

            // Addition
            makeMethodBinding<LiteralCharacterPtr (LargeInteger, LiteralCharacterPtr)> ("+", +[](const LargeInteger &a, const LiteralCharacterPtr &b) {
                auto result = std::make_shared<LiteralCharacter> ();
                result->value = a + b->getValue();
                return result;
            }, MethodFlags::Pure),

            makeMethodBinding<LiteralCharacterPtr (LiteralCharacterPtr, LargeInteger)> ("+", +[](const LiteralCharacterPtr &a, const LargeInteger &b) {
                auto result = std::make_shared<LiteralCharacter> ();
                result->value = a->getValue() + b;
                return result;
            }, MethodFlags::Pure),

            makeMethodBinding<LargeInteger (LargeInteger, LargeInteger)> ("+", +[](const LargeInteger &a, const LargeInteger &b) {
                return a + b;
            }, MethodFlags::Pure),

            makeMethodBinding<Fraction (LargeInteger, Fraction)> ("+", +[](const LargeInteger &a, const Fraction &b) {
                return Fraction{a} + b;
            }, MethodFlags::Pure),

            makeMethodBinding<double (LargeInteger, double)> ("+", +[](const LargeInteger &a, double b) {
                return a.asDouble() + b;
            }, MethodFlags::Pure),

            // Subtraction
            makeMethodBinding<LargeInteger (LargeInteger, LargeInteger)> ("-", +[](const LargeInteger &a, const LargeInteger &b) {
                return a - b;
            }, MethodFlags::Pure),

            makeMethodBinding<Fraction (LargeInteger, Fraction)> ("-", +[](const LargeInteger &a, const Fraction &b) {
                return Fraction{a} - b;
            }, MethodFlags::Pure),

            makeMethodBinding<double (LargeInteger, double)> ("-", +[](const LargeInteger &a, double b) {
                return a.asDouble() - b;
            }, MethodFlags::Pure),

            // Multiplication
            makeMethodBinding<LargeInteger (LargeInteger, LargeInteger)> ("*", +[](const LargeInteger &a, const LargeInteger &b) {
                return a * b;
            }, MethodFlags::Pure),

            makeMethodBinding<Fraction (LargeInteger, Fraction)> ("*", +[](const LargeInteger &a, const Fraction &b) {
                return Fraction{a} * b;
            }, MethodFlags::Pure),

            makeMethodBinding<double (LargeInteger, double)> ("*", +[](const LargeInteger &a, double b) {
                return a.asDouble() * b;
            }, MethodFlags::Pure),

            // Division
            makeMethodBinding<Fraction (LargeInteger, LargeInteger)> ("/", +[](const LargeInteger &a, const LargeInteger &b) {
                if(b.isZero())
                    signalNew<DivisionByZeroError> ();

                return Fraction{a, b}.reduced();
            }, MethodFlags::Pure),

            makeMethodBinding<Fraction (LargeInteger, Fraction)> ("/", +[](const LargeInteger &a, const Fraction &b) {
                if(b.numerator.isZero())
                    signalNew<DivisionByZeroError> ();

                return Fraction(a) / b;
            }, MethodFlags::Pure),

            makeMethodBinding<double (LargeInteger, double)> ("/", +[](const LargeInteger &a, double b) {
                return a.asDouble() / b;
            }, MethodFlags::Pure),

            // Integer division
            makeMethodBinding<LargeInteger (LargeInteger, LargeInteger)> ("//", +[](const LargeInteger &a, const LargeInteger &b) {
                if(b.isZero())
                    signalNew<DivisionByZeroError> ();
                return a / b;
            }, MethodFlags::Pure),

            // Remainder
            makeMethodBinding<LargeInteger (LargeInteger, LargeInteger)> ("%", +[](const LargeInteger &a, const LargeInteger &b) {
                if(b.isZero())
                    signalNew<DivisionByZeroError> ();

                return a % b;
            }, MethodFlags::Pure),

            makeMethodBinding<LargeInteger (LargeInteger, LargeInteger)> ("\\\\", +[](const LargeInteger &a, const LargeInteger &b) {
                if(b.isZero())
                    signalNew<DivisionByZeroError> ();

                return a % b;
            }, MethodFlags::Pure),
        }},

        {"math functions", {
            makeMethodBinding<LargeInteger (LargeInteger)> ("factorial", +[](const LargeInteger &value) {
                return value.factorial();
            }, MethodFlags::Pure)
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

LiteralCharacterPtr LiteralInteger::makeForCharacter(char32_t value)
{
    LiteralCharacterPtr result = std::make_shared<LiteralCharacter> ();
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
        signalNew<CannotUnwrap> ();
    return uint8_t(value.wordAt(0));
}

int8_t LiteralInteger::unwrapAsInt8() const
{
    if(value < LargeInteger{std::numeric_limits<int8_t>::min()} || value > LargeInteger{std::numeric_limits<int8_t>::max()})
        signalNew<CannotUnwrap> ();
    return value.isNegative() ? int8_t(-value.wordAt(0)) : int8_t(value.wordAt(0));
}

uint16_t LiteralInteger::unwrapAsUInt16() const
{
    if(value.isNegative() || value > LargeInteger{std::numeric_limits<uint16_t>::max()})
        signalNew<CannotUnwrap> ();
    return uint16_t(value.wordAt(0));
}

int16_t LiteralInteger::unwrapAsInt16() const
{
    if(value < LargeInteger{std::numeric_limits<int16_t>::min()} || value > LargeInteger{std::numeric_limits<int16_t>::max()})
        signalNew<CannotUnwrap> ();
    return value.isNegative() ? int16_t(-value.wordAt(0)) : int16_t(value.wordAt(0));
}

uint32_t LiteralInteger::unwrapAsUInt32() const
{
    if(value.isNegative() || value > LargeInteger{std::numeric_limits<uint32_t>::max()})
        signalNew<CannotUnwrap> ();
    return value.wordAt(0);
}

int32_t LiteralInteger::unwrapAsInt32() const
{
    if(value < LargeInteger{std::numeric_limits<int32_t>::min()} || value > LargeInteger{std::numeric_limits<int32_t>::max()})
        signalNew<CannotUnwrap> ();
    return value.isNegative() ? int32_t(-value.wordAt(0)) : int32_t(value.wordAt(0));
}

uint64_t LiteralInteger::unwrapAsUInt64() const
{
    if(value.isNegative() || value > LargeInteger{std::numeric_limits<uint64_t>::max()})
        signalNew<CannotUnwrap> ();
    return value.wordAt(0) | uint64_t(value.wordAt(1)) << 32;
}

int64_t LiteralInteger::unwrapAsInt64() const
{
    if(value < LargeInteger{std::numeric_limits<int64_t>::min()} || value > LargeInteger{std::numeric_limits<int64_t>::max()})
        signalNew<CannotUnwrap> ();

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
        signalNew<CannotUnwrap> ();
    return char(value.wordAt(0));
}

char16_t LiteralInteger::unwrapAsChar16() const
{
    if(value.isNegative() || value > LargeInteger{std::numeric_limits<char16_t>::max()})
        signalNew<CannotUnwrap> ();
    return char16_t(value.wordAt(0));
}

char32_t LiteralInteger::unwrapAsChar32() const
{
    if(value.isNegative() || value > LargeInteger{std::numeric_limits<char32_t>::max()})
        signalNew<CannotUnwrap> ();
    return char32_t(value.wordAt(0));
}

Fraction LiteralInteger::unwrapAsFraction() const
{
    return Fraction{value, LargeInteger{1}};
}

float LiteralInteger::unwrapAsFloat32() const
{

    signalNew<CannotUnwrap> ();
}

double LiteralInteger::unwrapAsFloat64() const
{
    return value.asDouble();
}

double LiteralInteger::asFloat() const
{
    return value.asDouble();
}

SExpression LiteralInteger::asSExpression() const
{
    return value;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius