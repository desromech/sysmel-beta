#include "Environment/Fraction.hpp"
#include "Environment/DivisionByZeroError.hpp"
#include "Environment/Assert.hpp"

namespace Sysmel
{
namespace Environment
{

Fraction Fraction::reduced() const
{
    if(numerator.isZero())
        return Fraction{LargeInteger{0}, LargeInteger{1}};
    
    auto gcd = LargeInteger::gcd(numerator, denominator);
    auto result = Fraction{numerator / gcd, denominator / gcd};
    result.numerator.signBit = result.numerator.signBit ^ result.denominator.signBit;
    result.denominator.signBit = false;
    sysmelAssert(result.numerator.isNormalized());
    sysmelAssert(result.denominator.isNormalized());
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
        signalNew<DivisionByZeroError> ();
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

} // End of namespace Environment
} // End of namespace Sysmel