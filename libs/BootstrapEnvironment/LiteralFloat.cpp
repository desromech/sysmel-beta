#include "sysmel/BootstrapEnvironment/LiteralFloat.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"
#include <algorithm>
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<LiteralFloat> literalFloatTypeRegistration;

TypePtr WrapperTypeFor<double>::apply()
{
    return LiteralFloat::__staticType__();
}

MethodCategories LiteralFloat::__instanceMethods__()
{
    return MethodCategories{
        {"arithmetic", {
            // Negation
            makeMethodBinding<double (double)> ("negated", +[](double value) {
                return -value;
            }),

            // Addition
            makeMethodBinding<double (double, LargeInteger)> ("+", +[](double a, const LargeInteger &b) {
                return a + b.asDouble();
            }),

            makeMethodBinding<double (double, Fraction)> ("+", +[](double a, const Fraction &b) {
                return a + b.asDouble();
            }),

            makeMethodBinding<double (double, double)> ("+", +[](double a, double b) {
                return a + b;
            }),

            // Subtraction
            makeMethodBinding<double (double, LargeInteger)> ("-", +[](double a, const LargeInteger &b) {
                return a - b.asDouble();
            }),

            makeMethodBinding<double (double, Fraction)> ("-", +[](double a, const Fraction &b) {
                return a - b.asDouble();
            }),

            makeMethodBinding<double (double, double)> ("-", +[](double a, double b) {
                return a - b;
            }),

            // Multiplication
            makeMethodBinding<double (double, LargeInteger)> ("*", +[](double a, const LargeInteger &b) {
                return a * b.asDouble();
            }),

            makeMethodBinding<double (double, Fraction)> ("*", +[](double a, const Fraction &b) {
                return a * b.asDouble();
            }),

            makeMethodBinding<double (double, double)> ("*", +[](double a, double b) {
                return a * b;
            }),

            // Division
            makeMethodBinding<double (double, LargeInteger)> ("/", +[](double a, const LargeInteger &b) {
                return a / b.asDouble();
            }),

            makeMethodBinding<double (double, Fraction)> ("/", +[](double a, const Fraction &b) {
                return a / b.asDouble();
            }),

            makeMethodBinding<double (double, double)> ("/", +[](double a, double b) {
                return a / b;
            }),
        }}
    };
}

bool LiteralFloat::isLiteralFloat() const
{
    return true;
}

std::string LiteralFloat::printString() const
{
    std::ostringstream out;
    out << value;
    return out.str();
}

float LiteralFloat::unwrapAsFloat32() const
{
    if(float(value) != value)
        throw CannotUnwrap("Cannot unwrap float32 because of loss of precision.");
    return value;
}

double LiteralFloat::unwrapAsFloat64() const
{
    return value;
}

double LiteralFloat::asFloat() const
{
    return value;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius