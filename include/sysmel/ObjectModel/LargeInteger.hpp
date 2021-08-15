#ifndef SYSMEL_OBJECT_MODEL_LARGE_INTEGER
#define SYSMEL_OBJECT_MODEL_LARGE_INTEGER
#pragma once

#include <stdint.h>
#include <string>
#include <ostream>
#include <vector>
#include <cassert>

namespace SysmelMoebius
{
namespace ObjectModel
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

    LargeInteger(uint32_t value);
    LargeInteger(int32_t value);
    LargeInteger(uint64_t value);
    LargeInteger(int64_t value);
    LargeInteger(bool isNegative, std::vector<uint32_t> &&newWords);
    LargeInteger(bool isNegative, const std::vector<uint32_t> &newWords);
    LargeInteger(const std::string &string, uint8_t radix = 10);

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

    LargeInteger operator-() const;


    LargeInteger operator+(const LargeInteger &other) const;
    const LargeInteger &operator+=(const LargeInteger &other);

    LargeInteger operator-(const LargeInteger &other) const;
    const LargeInteger &operator-=(const LargeInteger &other);

    LargeInteger operator*(const LargeInteger &other) const;
    const LargeInteger &operator*=(const LargeInteger &other);

    LargeInteger operator<<(uint32_t shiftAmount) const;
    const LargeInteger &operator<<=(uint32_t shiftAmount);

    LargeInteger operator>>(uint32_t shiftAmount) const;
    const LargeInteger &operator>>=(uint32_t shiftAmount);

    LargeInteger factorial() const;
    void divisionAndRemainder(const LargeInteger &divisor, LargeInteger &quotient, LargeInteger &remainder);

    bool isZero() const;
    bool isOne() const;
    bool isMinusOne() const;

    std::string asString() const;
    void normalize();

    friend std::ostream &operator<<(std::ostream &out, const LargeInteger &integer)
    {
        return out << integer.asString();
    }
};

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_OBJECT_MODEL_LARGE_INTEGER