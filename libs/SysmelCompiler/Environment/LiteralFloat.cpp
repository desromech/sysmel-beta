#include "Environment/LiteralFloat.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/CannotUnwrap.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<LiteralFloat> literalFloatTypeRegistration;

TypePtr WrapperTypeFor<double>::apply()
{
    return LiteralFloat::__staticType__();
}

MethodCategories LiteralFloat::__instanceMethods__()
{
    return MethodCategories{
        {"comparisons", {
            // =
            makeMethodBinding<bool (double, LargeInteger)> ("=", +[](double a, const LargeInteger &b) {
                return a == b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, Fraction)> ("=", +[](double a, const Fraction &b) {
                return a == b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, double)> ("=", +[](double a, double b) {
                return a == b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, AnyValuePtr)> ("=", +[](double, const AnyValuePtr &) {
                return false;
            }, MethodFlags::Pure),

            // ~=
            makeMethodBinding<bool (double, LargeInteger)> ("~=", +[](double a, const LargeInteger &b) {
                return a != b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, Fraction)> ("~=", +[](double a, const Fraction &b) {
                return a != b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, double)> ("~=", +[](double a, double b) {
                return a != b;
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, AnyValuePtr)> ("~=", +[](double, const AnyValuePtr &) {
                return true;
            }, MethodFlags::Pure),

            // <
            makeMethodBinding<bool (double, LargeInteger)> ("<", +[](double a, const LargeInteger &b) {
                return a < b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, Fraction)> ("<", +[](double a, const Fraction &b) {
                return a < b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, double)> ("<", +[](double a, double b) {
                return a < b;
            }, MethodFlags::Pure),

            // <=
            makeMethodBinding<bool (double, LargeInteger)> ("<=", +[](double a, const LargeInteger &b) {
                return a <= b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, Fraction)> ("<=", +[](double a, const Fraction &b) {
                return a <= b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, double)> ("<=", +[](double a, double b) {
                return a <= b;
            }, MethodFlags::Pure),

            // >
            makeMethodBinding<bool (double, LargeInteger)> (">", +[](double a, const LargeInteger &b) {
                return a > b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, Fraction)> (">", +[](double a, const Fraction &b) {
                return a > b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, double)> (">", +[](double a, double b) {
                return a > b;
            }, MethodFlags::Pure),

            // >=
            makeMethodBinding<bool (double, LargeInteger)> (">=", +[](double a, const LargeInteger &b) {
                return a >= b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, Fraction)> (">=", +[](double a, const Fraction &b) {
                return a >= b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<bool (double, double)> (">=", +[](double a, double b) {
                return a >= b;
            }, MethodFlags::Pure),
            
        }},
        {"arithmetic", {
            // Negation
            makeMethodBinding<double (double)> ("negated", +[](double value) {
                return -value;
            }, MethodFlags::Pure),
            makeMethodBinding<double (double)> ("pre--", +[](double value) {
                return -value;
            }, MethodFlags::Pure),

            // Addition
            makeMethodBinding<double (double, LargeInteger)> ("+", +[](double a, const LargeInteger &b) {
                return a + b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<double (double, Fraction)> ("+", +[](double a, const Fraction &b) {
                return a + b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<double (double, double)> ("+", +[](double a, double b) {
                return a + b;
            }, MethodFlags::Pure),

            // Subtraction
            makeMethodBinding<double (double, LargeInteger)> ("-", +[](double a, const LargeInteger &b) {
                return a - b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<double (double, Fraction)> ("-", +[](double a, const Fraction &b) {
                return a - b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<double (double, double)> ("-", +[](double a, double b) {
                return a - b;
            }, MethodFlags::Pure),

            // Multiplication
            makeMethodBinding<double (double, LargeInteger)> ("*", +[](double a, const LargeInteger &b) {
                return a * b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<double (double, Fraction)> ("*", +[](double a, const Fraction &b) {
                return a * b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<double (double, double)> ("*", +[](double a, double b) {
                return a * b;
            }, MethodFlags::Pure),

            // Division
            makeMethodBinding<double (double, LargeInteger)> ("/", +[](double a, const LargeInteger &b) {
                return a / b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<double (double, Fraction)> ("/", +[](double a, const Fraction &b) {
                return a / b.asDouble();
            }, MethodFlags::Pure),

            makeMethodBinding<double (double, double)> ("/", +[](double a, double b) {
                return a / b;
            }, MethodFlags::Pure),
        }}
    };
}

bool LiteralFloat::isLiteralFloat() const
{
    return true;
}

AnyValuePtr LiteralFloat::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitLiteralFloat(selfFromThis());
}

std::string LiteralFloat::printString() const
{
    std::ostringstream out;
    out << value;
    if(value == floor(value))
        out << ".0";

    return out.str();
}

float LiteralFloat::unwrapAsFloat32() const
{
    if(float(value) != value)
        signalNewWithMessage<CannotUnwrap> ("Cannot unwrap float32 because of loss of precision.");
    return float(value);
}

double LiteralFloat::unwrapAsFloat64() const
{
    return value;
}

double LiteralFloat::asFloat() const
{
    return value;
}

SExpression LiteralFloat::asSExpression() const
{
    return value;
}

} // End of namespace Environment
} // End of namespace Sysmel