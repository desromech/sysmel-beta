#include "sysmel/ObjectModel/Fraction.hpp"
#include <assert.h>

namespace SysmelMoebius
{
namespace ObjectModel
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

double Fraction::asDouble() const
{
    return numerator.asDouble() / denominator.asDouble();
}

float Fraction::asFloat() const
{
    return float(asDouble());
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius