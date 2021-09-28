#ifndef SYSMEL_BOOTSTRAP_ENVIRONMENT_LARGE_INTEGER_HPP
#define SYSMEL_BOOTSTRAP_ENVIRONMENT_LARGE_INTEGER_HPP
#pragma once

#include <stdint.h>
#include <string>
#include <ostream>
#include <vector>
#include <cassert>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I represent a large integer value.
 */
struct LargeInteger
{
    bool signBit = 0;
    std::vector<uint32_t> words;

    static const LargeInteger Zero;
    static const LargeInteger One;
    static const LargeInteger MinusOne;
    static const LargeInteger Ten;

    LargeInteger()
    {
    }

    explicit LargeInteger(uint32_t value);
    explicit LargeInteger(int32_t value);
    explicit LargeInteger(uint64_t value);
    explicit LargeInteger(int64_t value);
    explicit LargeInteger(bool isNegative, std::vector<uint32_t> &&newWords);
    explicit LargeInteger(bool isNegative, const std::vector<uint32_t> &newWords);
    explicit LargeInteger(const std::string &string, uint8_t radix = 10);

    void setValue(uint32_t value);
    void setValue(int32_t value);
    void setValue(uint64_t value);
    void setValue(int64_t value);
    void setValueByParsingFrom(const std::string &string, uint8_t radix = 10);

    template<size_t N>
    void setUnnormalizedWords(const uint32_t (&words)[N])
    {
        setUnnormalizedWords(words, N);
    }

    void setUnnormalizedWords(const std::vector<uint32_t> &newWords);
    void setUnnormalizedWords(const uint32_t newWords[], size_t wordCount);
    int32_t compareWith(const LargeInteger &other) const;

    uint32_t wordAt(size_t index) const
    {
        if(index < words.size())
            return words[index];
        return 0;
    }

    int sign() const
    {
        if(words.empty())
            return 0;
        return signBit ? -1 : 1;
    }

    bool operator<(const LargeInteger &other) const
    {
        return compareWith(other) < 0;
    }

    bool operator<=(const LargeInteger &other) const
    {
        return compareWith(other) <= 0;
    }

    bool operator>=(const LargeInteger &other) const
    {
        return compareWith(other) >= 0;
    }

    bool operator>(const LargeInteger &other) const
    {
        return compareWith(other) > 0;
    }

    bool operator==(const LargeInteger &other) const
    {
        return compareWith(other) == 0;
    }

    bool operator!=(const LargeInteger &other) const
    {
        return compareWith(other) != 0;
    }

    const LargeInteger &operator+() const
    {
        return *this;
    }

    LargeInteger abs() const;
    
    LargeInteger operator-() const;
    LargeInteger operator~() const;

    LargeInteger operator+(const LargeInteger &other) const;
    LargeInteger &operator+=(const LargeInteger &other);

    LargeInteger operator-(const LargeInteger &other) const;
    LargeInteger &operator-=(const LargeInteger &other);

    LargeInteger operator*(const LargeInteger &other) const;
    LargeInteger &operator*=(const LargeInteger &other);

    LargeInteger operator/(const LargeInteger &divisor) const;
    LargeInteger &operator/=(const LargeInteger &divisor);

    LargeInteger operator%(const LargeInteger &divisor) const;
    LargeInteger &operator%=(const LargeInteger &divisor);

    LargeInteger operator<<(uint32_t shiftAmount) const;
    LargeInteger &operator<<=(uint32_t shiftAmount);

    LargeInteger operator>>(uint32_t shiftAmount) const;
    LargeInteger &operator>>=(uint32_t shiftAmount);

    LargeInteger factorial() const;
    static LargeInteger binomialCoefficient(const LargeInteger &n, const LargeInteger &k);

    void divisionAndRemainder(const LargeInteger &divisor, LargeInteger &quotient, LargeInteger &remainder) const;

    bool isNegative() const
    {
        return signBit && !words.empty();
    }

    bool isNormalized() const
    {
        return words.empty() || words.back() != 0;
    }

    bool isZero() const;
    bool isOne() const;
    bool isMinusOne() const;

    uint32_t highBitOfMagnitude() const;
    double asDouble() const;

    std::string asHexString() const;
    std::string asString() const;
    void normalize();

    static LargeInteger gcd(const LargeInteger &a, const LargeInteger &b);

    friend std::ostream &operator<<(std::ostream &out, const LargeInteger &integer)
    {
        return out << integer.asString();
    }
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_BOOTSTRAP_ENVIRONMENT_LARGE_INTEGER_HPP
