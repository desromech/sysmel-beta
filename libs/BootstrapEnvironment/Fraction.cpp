#include "sysmel/BootstrapEnvironment/Fraction.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"
#include <assert.h>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

Fraction Fraction::reduced() const
{
    if(numerator.isZero())
        return Fraction{LargeInteger{0}, LargeInteger{1}};
    
    auto gcd = LargeInteger::gcd(numerator, denominator);
    auto result = Fraction{numerator / gcd, denominator / gcd};
    result.numerator.signBit = result.numerator.signBit ^ result.denominator.signBit;
    result.denominator.signBit = false;
    assert(result.numerator.isNormalized());
    assert(result.denominator.isNormalized());
    return result;
}

Fraction Fraction::operator-() const
{
    return Fraction{-numerator, denominator};
}

Fraction Fraction::operator+(const Fraction &other) const
{
    return Fraction{numerator * other.denominator + denominator*other.numerator, denominator * other.denominator}.reduced();
}

Fraction Fraction::operator-(const Fraction &other) const
{
    return Fraction{numerator * other.denominator - denominator*other.numerator, denominator * other.denominator}.reduced();
}

Fraction Fraction::operator*(const Fraction &other) const
{
    return Fraction{numerator * other.numerator, denominator * other.denominator}.reduced();
}

Fraction Fraction::operator/(const Fraction &other) const
{
    if(other.numerator.isZero())
        throw DivisionByZeroError();
    return Fraction{numerator * other.denominator, denominator * other.numerator}.reduced();
}

bool Fraction::operator==(const Fraction &other) const
{
    return numerator*other.denominator == denominator*other.numerator;
}

bool Fraction::operator!=(const Fraction &other) const
{
    return numerator*other.denominator != denominator*other.numerator;
}

bool Fraction::operator<(const Fraction &other) const
{
    return numerator*other.denominator < denominator*other.numerator;
}

bool Fraction::operator<=(const Fraction &other) const
{
    return numerator*other.denominator <= denominator*other.numerator;
}

bool Fraction::operator>(const Fraction &other) const
{
    return numerator*other.denominator > denominator*other.numerator;
}

bool Fraction::operator>=(const Fraction &other) const
{
    return numerator*other.denominator >= denominator*other.numerator;
}

double Fraction::asDouble() const
{
    return numerator.asDouble() / denominator.asDouble();
}

float Fraction::asFloat() const
{
    return float(asDouble());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius