#ifndef SYSMEL_OBJECT_MODEL_FRACTION_HPP
#define SYSMEL_OBJECT_MODEL_FRACTION_HPP
#pragma once

#include "LargeInteger.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I represent a fraction
 */
struct Fraction
{
    LargeInteger numerator;
    LargeInteger denominator;
    
    Fraction reduced() const;

    float asFloat() const;
    double asDouble() const;

    friend std::ostream &operator<<(std::ostream &out, const Fraction &fraction)
    {
        return out << fraction.numerator << '/' << fraction.denominator;
    }
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_OBJECT_MODEL_FRACTION_HPP
