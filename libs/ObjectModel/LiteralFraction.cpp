#include "sysmel/ObjectModel/LiteralFraction.hpp"
#include "sysmel/ObjectModel/LiteralInteger.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace ObjectModel
{

static BootstrapTypeRegistration<LiteralFraction> literalFractionTypeRegistration;

TypePtr WrapperTypeFor<Fraction>::apply()
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius