#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_FRACTION_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_FRACTION_HPP
#pragma once

#include "LargeInteger.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I represent a fraction
 */
struct Fraction
{
    LargeInteger numerator;
    LargeInteger denominator;

    Fraction() = default;

    Fraction(LargeInteger initialNumerator, LargeInteger initialDenominator)
        : numerator(initialNumerator), denominator(initialDenominator) {}

    Fraction(int initialNumerator, int initialDenominator)
        : numerator(LargeInteger(initialNumerator)), denominator(LargeInteger(initialDenominator)) {}

    Fraction(LargeInteger integerPart)
        : numerator(integerPart), denominator(LargeInteger::One) {}

    Fraction reduced() const;

    Fraction operator-() const;
    Fraction operator+(const Fraction &other) const;
    Fraction operator-(const Fraction &other) const;
    Fraction operator*(const Fraction &other) const;
    Fraction operator/(const Fraction &other) const;

    bool operator==(const Fraction &other) const;
    bool operator!=(const Fraction &other) const;
    bool operator<(const Fraction &other) const;
    bool operator<=(const Fraction &other) const;
    bool operator>(const Fraction &other) const;
    bool operator>=(const Fraction &other) const;

    float asFloat() const;
    double asDouble() const;

    friend std::ostream &operator<<(std::ostream &out, const Fraction &fraction)
    {
        return out << fraction.numerator << '/' << fraction.denominator;
    }
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_FRACTION_HPP
