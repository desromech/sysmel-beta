#include "sysmel/BootstrapEnvironment/LiteralFraction.hpp"
#include "sysmel/BootstrapEnvironment/LiteralInteger.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
        {"arithmetic", {
            makeMethodBinding<Fraction (Fraction)> ("negated", +[](const Fraction &value) {
                return -value;
            }),

            // Addition
            makeMethodBinding<Fraction (Fraction, LargeInteger)> ("+", +[](const Fraction &a, const LargeInteger &b) {
                return a + Fraction{b};
            }),

            makeMethodBinding<Fraction (Fraction, Fraction)> ("+", +[](const Fraction &a, const Fraction &b) {
                return a + Fraction{b};
            }),

            makeMethodBinding<double (Fraction, double)> ("+", +[](const Fraction &a, double b) {
                return a.asDouble() + b;
            }),

            // Subtraction
            makeMethodBinding<Fraction (Fraction, LargeInteger)> ("-", +[](const Fraction &a, const LargeInteger &b) {
                return a - Fraction{b};
            }),

            makeMethodBinding<Fraction (Fraction, Fraction)> ("-", +[](const Fraction &a, const Fraction &b) {
                return a - Fraction{b};
            }),

            makeMethodBinding<double (Fraction, double)> ("-", +[](const Fraction &a, double b) {
                return a.asDouble() - b;
            }),

            // Multiplication
            makeMethodBinding<Fraction (Fraction, LargeInteger)> ("*", +[](const Fraction &a, const LargeInteger &b) {
                return a * Fraction{b};
            }),

            makeMethodBinding<Fraction (Fraction, Fraction)> ("*", +[](const Fraction &a, const Fraction &b) {
                return a * Fraction{b};
            }),

            makeMethodBinding<double (Fraction, double)> ("*", +[](const Fraction &a, double b) {
                return a.asDouble() * b;
            }),

            // Division
            makeMethodBinding<Fraction (Fraction, LargeInteger)> ("/", +[](const Fraction &a, const LargeInteger &b) {
                return a / Fraction{b};
            }),

            makeMethodBinding<Fraction (Fraction, Fraction)> ("/", +[](const Fraction &a, const Fraction &b) {
                return a / Fraction{b};
            }),

            makeMethodBinding<double (Fraction, double)> ("/", +[](const Fraction &a, double b) {
                return a.asDouble() / b;
            }),
        }}
    };
}

LiteralNumberPtr LiteralFraction::makeFor(const Fraction &value)
{
    if(value.denominator.isZero())
        throw DivisionByZeroError();
    else if(value.denominator.isOne())
        return LiteralInteger::makeFor(value.numerator);
    
    auto result = std::make_shared<LiteralFraction> ();
    result->value = value;
    return result;
}

bool LiteralFraction::isLiteralFraction() const
{
    return true;
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius