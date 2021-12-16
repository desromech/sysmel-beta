#include "Environment/LiteralFraction.hpp"
#include "Environment/LiteralInteger.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/DivisionByZeroError.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<LiteralFraction> literalFractionTypeRegistration;

TypePtr WrapperTypeFor<Fraction>::apply()
{
    return LiteralFraction::__staticType__();
}

TypePtr WrapperTypeForReturning<Fraction>::apply()
{
    return LiteralNumber::__staticType__();
}

MethodCategories LiteralFraction::__instanceMethods__()
{
    return MethodCategories{
        {"comparisons", {
            // =
            makeMethodBinding<bool (Fraction, LargeInteger)> ("=", +[](const Fraction &a, const LargeInteger &b) {
                return a == Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, Fraction)> ("=", +[](const Fraction &a, const Fraction &b) {
                return a == b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, double)> ("=", +[](const Fraction &a, double b) {
                return a.asDouble() == b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, AnyValuePtr)> ("=", +[](const Fraction&, const AnyValuePtr &) {
                return false;
            }, MethodFlags::Pure),

            // ~=
            makeMethodBinding<bool (Fraction, LargeInteger)> ("~=", +[](const Fraction &a, const LargeInteger &b) {
                return a != Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, Fraction)> ("~=", +[](const Fraction &a, const Fraction &b) {
                return a != b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, double)> ("~=", +[](const Fraction &a, double b) {
                return a.asDouble() != b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, AnyValuePtr)> ("~=", +[](const Fraction&, const AnyValuePtr &) {
                return true;
            }, MethodFlags::Pure),

            // <
            makeMethodBinding<bool (Fraction, LargeInteger)> ("<", +[](const Fraction &a, const LargeInteger &b) {
                return a < Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, Fraction)> ("<", +[](const Fraction &a, const Fraction &b) {
                return a < b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, double)> ("<", +[](const Fraction &a, double b) {
                return a.asDouble() < b;
            }, MethodFlags::Pure),

            // <=
            makeMethodBinding<bool (Fraction, LargeInteger)> ("<=", +[](const Fraction &a, const LargeInteger &b) {
                return a <= Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, Fraction)> ("<=", +[](const Fraction &a, const Fraction &b) {
                return a <= b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, double)> ("<=", +[](const Fraction &a, double b) {
                return a.asDouble() <= b;
            }, MethodFlags::Pure),

            // >
            makeMethodBinding<bool (Fraction, LargeInteger)> (">", +[](const Fraction &a, const LargeInteger &b) {
                return a > Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, Fraction)> (">", +[](const Fraction &a, const Fraction &b) {
                return a > b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, double)> (">", +[](const Fraction &a, double b) {
                return a.asDouble() > b;
            }, MethodFlags::Pure),

            // >=
            makeMethodBinding<bool (Fraction, LargeInteger)> (">=", +[](const Fraction &a, const LargeInteger &b) {
                return a >= Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, Fraction)> (">=", +[](const Fraction &a, const Fraction &b) {
                return a >= b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (Fraction, double)> (">=", +[](const Fraction &a, double b) {
                return a.asDouble() >= b;
            }, MethodFlags::Pure),
        }},

        {"arithmetic", {
            // Negation
            makeMethodBinding<Fraction (Fraction)> ("negated", +[](const Fraction &value) {
                return -value;
            }, MethodFlags::Pure),
            makeMethodBinding<Fraction (Fraction)> ("pre--", +[](const Fraction &value) {
                return -value;
            }, MethodFlags::Pure),

            // Addition
            makeMethodBinding<Fraction (Fraction, LargeInteger)> ("+", +[](const Fraction &a, const LargeInteger &b) {
                return a + Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<Fraction (Fraction, Fraction)> ("+", +[](const Fraction &a, const Fraction &b) {
                return a + Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<double (Fraction, double)> ("+", +[](const Fraction &a, double b) {
                return a.asDouble() + b;
            }, MethodFlags::Pure),

            // Subtraction
            makeMethodBinding<Fraction (Fraction, LargeInteger)> ("-", +[](const Fraction &a, const LargeInteger &b) {
                return a - Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<Fraction (Fraction, Fraction)> ("-", +[](const Fraction &a, const Fraction &b) {
                return a - Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<double (Fraction, double)> ("-", +[](const Fraction &a, double b) {
                return a.asDouble() - b;
            }, MethodFlags::Pure),

            // Multiplication
            makeMethodBinding<Fraction (Fraction, LargeInteger)> ("*", +[](const Fraction &a, const LargeInteger &b) {
                return a * Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<Fraction (Fraction, Fraction)> ("*", +[](const Fraction &a, const Fraction &b) {
                return a * Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<double (Fraction, double)> ("*", +[](const Fraction &a, double b) {
                return a.asDouble() * b;
            }, MethodFlags::Pure),

            // Division
            makeMethodBinding<Fraction (Fraction, LargeInteger)> ("/", +[](const Fraction &a, const LargeInteger &b) {
                return a / Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<Fraction (Fraction, Fraction)> ("/", +[](const Fraction &a, const Fraction &b) {
                return a / Fraction{b};
            }, MethodFlags::Pure),

            makeMethodBinding<double (Fraction, double)> ("/", +[](const Fraction &a, double b) {
                return a.asDouble() / b;
            }, MethodFlags::Pure),
        }}
    };
}

LiteralNumberPtr LiteralFraction::makeFor(const Fraction &value)
{
    if(value.denominator.isZero())
        signalNew<DivisionByZeroError> ();
    else if(value.denominator.isOne())
        return LiteralInteger::makeFor(value.numerator);
    
    auto result = basicMakeObject<LiteralFraction> ();
    result->value = value;
    return result;
}

bool LiteralFraction::isLiteralFraction() const
{
    return true;
}

AnyValuePtr LiteralFraction::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitLiteralFraction(selfFromThis());
}

std::string LiteralFraction::printString() const
{
    std::ostringstream out;
    out << value;
    return out.str();
}

Fraction LiteralFraction::unwrapAsFraction() const
{
    return value;
}

float LiteralFraction::unwrapAsFloat32() const
{
    return value.asFloat();
}

double LiteralFraction::unwrapAsFloat64() const
{
    return value.asDouble();
}

double LiteralFraction::asFloat() const
{
    return value.asDouble();
}

SExpression LiteralFraction::asSExpression() const
{
    return value;
}

} // End of namespace Environment
} // End of namespace Sysmel