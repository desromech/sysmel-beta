#include "sysmel/ObjectModel/LargeInteger.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace ObjectModel
{

static uint32_t highBitOf(uint32_t word)
{
#if defined(_MSC_VER)
    return 32 - __lzcnt
#elif defined(__GNUC__)
    return 32 - __builtin_clz(word);
#else
    uint32_t result = 0;
    while(word > 0)
    {
        ++result;
        word >>= 1;
    }

    return result;
#endif
}

static bool isAllZeroWords(const uint32_t words[], size_t wordCount)
{
    for(size_t i = 0; i < wordCount; ++i )
    {
        if(words[i] != 0)
            return false;
    }

    return true;
}
static size_t computeSignificantWordCount(const uint32_t words[], size_t wordCount)
{
    while(wordCount > 0)
    {
        if(words[wordCount - 1] == 0)
            --wordCount;
        else
            break;
    }

    return wordCount;
}

static int32_t compareMagnitudes(const LargeInteger &left, const LargeInteger &right, size_t rightShift = 0)
{
    intptr_t wordCountDiff = left.words.size() - (right.words.size() + rightShift);
    if(wordCountDiff != 0)
        return wordCountDiff;

    auto wordCount = left.words.size();
    for(size_t i = 0; i < wordCount; ++i)
    {
        auto wordIndex = wordCount - i - 1;
        auto leftWord = left.words[wordIndex + rightShift];
        auto rightWord = right.words[wordIndex];
        auto difference = static_cast<int32_t> (leftWord - rightWord);
        if(difference != 0)
            return difference;
    }

    return 0;
}

static void sumMagnitudesInto(const LargeInteger &left, const LargeInteger &right, LargeInteger &result)
{
    auto resultWordCount = std::max(left.words.size(), right.words.size());
    result.words.clear();
    result.words.reserve(resultWordCount + 1);

    uint32_t carry = 0;
    for(size_t i = 0; i < resultWordCount; ++i)
    {
        uint64_t leftWord = left.wordAt(i);
        uint64_t rightWord = right.wordAt(i);
        uint64_t sum = leftWord + rightWord + carry;
        result.words.push_back(static_cast<uint32_t> (sum & 0xFFFFFFFF));
        carry = static_cast<uint32_t> (sum >> 32);
    }

    if(carry != 0)
        result.words.push_back(carry);
}

static void subtractMagnitudesInto(const LargeInteger &left, const LargeInteger &right, LargeInteger &result)
{
    auto resultWordCount = std::max(left.words.size(), right.words.size());
    result.words.clear();
    result.words.reserve(resultWordCount);

    int32_t borrow = 0;
    for(size_t i = 0; i < resultWordCount; ++i)
    {
        uint64_t leftWord = left.wordAt(i);
        uint64_t rightWord = right.wordAt(i);
        int64_t subtraction = leftWord - rightWord + borrow;
        result.words.push_back(static_cast<uint32_t> (subtraction & 0xFFFFFFFF));
        borrow = static_cast<int32_t> (subtraction >> 32);
    }
}

static void accumulateOffsetTimesWordInto(const LargeInteger &left, size_t offset, uint32_t rightWord, LargeInteger &accumulator)
{
    uint32_t carry = 0;
    for(size_t i = 0; i < left.words.size(); ++i)
    {
        uint64_t leftWord = left.words[i];
        auto &accumulatorWord = accumulator.words[i + offset];
        uint64_t sum = leftWord*rightWord + accumulatorWord + carry;
        accumulatorWord = static_cast<uint32_t> (sum & 0xFFFFFFFF);
        carry = static_cast<uint32_t> (sum >> 32);
    }

    // Keep propagating the carry beyond.
    for(size_t i = offset + left.words.size(); i < accumulator.words.size(); ++i)
    {
        auto &accumulatorWord = accumulator.words[i];
        uint64_t sum = uint64_t(accumulatorWord) + carry;
        accumulatorWord = static_cast<uint32_t> (sum & 0xFFFFFFFF);
        carry = static_cast<uint32_t> (sum >> 32);
    }
}

static void multiplyAndAddBySingleWordInPlace(LargeInteger &leftAndResult, uint32_t rightWord, uint32_t startCarry)
{
    uint32_t carry = startCarry;
    for(size_t i = 0; i < leftAndResult.words.size(); ++i)
    {
        auto &resultWord = leftAndResult.words[i];
        uint64_t sum = uint64_t(resultWord)*rightWord + carry;
        resultWord = static_cast<uint32_t> (sum & 0xFFFFFFFF);
        carry = static_cast<uint32_t> (sum >> 32);
    }

    if(carry > 0)
        leftAndResult.words.push_back(carry);
}

static void multiplyMagnitudesInto(const LargeInteger &left, const LargeInteger &right, LargeInteger &result)
{
    auto resultWordCount = left.words.size() + right.words.size();
    result.words.clear();
    result.words.resize(resultWordCount);

    for(size_t i = 0; i < right.words.size(); ++i)
    {
        accumulateOffsetTimesWordInto(left, i, right.words[i], result);
    }
}

static void accumulateSubtractingMultipliedWithOffset(LargeInteger &accumulator, const LargeInteger &right, uint32_t wordFactor, size_t offset)
{
    assert(right.words.size() + offset <= accumulator.words.size());

    int32_t borrow = 0;
    for(size_t i = 0; i < right.words.size(); ++i)
    {
        auto &accumulatorWord = accumulator.words[i + offset];
        uint64_t rightWord = uint64_t(right.words[i])*wordFactor;

        int64_t subtraction = accumulatorWord - rightWord + borrow;
        accumulatorWord = static_cast<uint32_t> (subtraction & 0xFFFFFFFF);
        borrow = static_cast<int32_t> (subtraction >> 32);
    }

    // Keep propagating the carry beyond.
    for(size_t i = right.words.size() + offset; i < accumulator.words.size(); ++i)
    {
        auto &accumulatorWord = accumulator.words[i];
        int64_t subtraction = uint64_t(accumulatorWord) + borrow;
        accumulatorWord = static_cast<uint32_t> (subtraction & 0xFFFFFFFF);
        borrow = static_cast<int32_t> (subtraction >> 32);
    }

    // If there is still some borrow, flip the sign bit, and assign to the borrow.
    if(borrow != 0)
    {
        accumulator.signBit = !accumulator.signBit;
        accumulator.words.resize(1);
        accumulator.words[0] = borrow;
    }
}

static void divideMagnitudesInto(const LargeInteger &dividend, const LargeInteger &divisor, LargeInteger &quotient, LargeInteger &remainder)
{
    auto n = divisor.words.size();
    auto m = dividend.words.size() - n;
    quotient.words.clear();
    quotient.words.resize(m + 1);

    remainder = dividend;
    remainder.signBit = false;

    if(compareMagnitudes(dividend, divisor, m) >= 0)
    {
        quotient.words[m] = 1;
        accumulateSubtractingMultipliedWithOffset(remainder, divisor, 1, m);
        remainder.normalize();
    }
    else
    {
        quotient.words[m] = 0;
    }

    auto lastDivisorDigit = divisor.words.back();

    for(auto i = m; i > 0;)
    {
        assert(!remainder.signBit);
        --i;

        auto &qi = quotient.words[i];
        auto qs = ((uint64_t(remainder.words[i + n]) << 32) | remainder.words[i + n - 1]) / lastDivisorDigit;
        qi = uint32_t(std::min(qs, uint64_t(0xFFFFFFFF)));
        accumulateSubtractingMultipliedWithOffset(remainder, divisor, qi, i);
        remainder.normalize();
        if(remainder.signBit)
        {
            --qi;
            accumulateSubtractingMultipliedWithOffset(remainder, divisor, 1, i);
            remainder.normalize();
        }

        assert(!remainder.signBit);
    }
}

static uint8_t bitsPerDigitInRadix(uint8_t radix)
{
    if(radix == 0)
        return 0;

    return highBitOf(radix - 1) + 1;
}

static int8_t parseDigitInRadix(char digit, uint8_t radix)
{
    int8_t parsedDigit = -1;

    if('0' <= digit && digit <= '9')
        parsedDigit = digit - '0';
    else if('A' <= digit && digit <= 'Z')
        parsedDigit = digit - 'A' + 10;
    else if('a' <= digit && digit <= 'z')
        parsedDigit = digit - 'a' + 10;

    // Make sure the parsed digit is in the radix range.
    if(parsedDigit >= int32_t(radix))
        parsedDigit = -1;
    return parsedDigit;
}

const LargeInteger LargeInteger::Zero = LargeInteger{0};
const LargeInteger LargeInteger::One = LargeInteger{1};
const LargeInteger LargeInteger::MinusOne = LargeInteger{-1};
const LargeInteger LargeInteger::Ten = LargeInteger{10};

LargeInteger::LargeInteger(uint32_t value)
{
    setValue(value);
}

LargeInteger::LargeInteger(int32_t value)
{
    setValue(value);
}

LargeInteger::LargeInteger(uint64_t value)
{
    setValue(value);
}

LargeInteger::LargeInteger(int64_t value)
{
    setValue(value);
}

LargeInteger::LargeInteger(bool isNegative, const std::vector<uint32_t> &newWords)
{
    signBit = isNegative;
    setUnnormalizedWords(newWords);
}

LargeInteger::LargeInteger(bool isNegative, std::vector<uint32_t> &&newWords)
{
    signBit = isNegative;
    words = std::move(newWords);
    normalize();
}

LargeInteger::LargeInteger(const std::string &string, uint8_t radix)
{
    setValueByParsingFrom(string, radix);
}

void LargeInteger::setValue(uint32_t value)
{
    signBit = false;
    uint32_t words[] = {
        value,
    };
    setUnnormalizedWords(words);
}

void LargeInteger::setValue(int32_t value)
{
    signBit = value < 0;
    uint32_t words[] = {
        static_cast<uint32_t> (value < 0 ? -value : value),
    };
    setUnnormalizedWords(words);
}

void LargeInteger::setValue(uint64_t value)
{
    signBit = 0;
    uint32_t words[] = {
        static_cast<uint32_t> (value & 0xffffffff),
        static_cast<uint32_t> (value >> 32),
    };
    setUnnormalizedWords(words);
}

void LargeInteger::setValue(int64_t value)
{
    signBit = value < 0;
    uint64_t absValue = value < 0 ? -value : value;
    uint32_t words[] = {
        static_cast<uint32_t> (absValue & 0xffffffff),
        static_cast<uint32_t> (absValue >> 32),
    };
    setUnnormalizedWords(words);
}

void LargeInteger::setValueByParsingFrom(const std::string &string, uint8_t radix)
{
    // Clear myself.
    *this = Zero;

    if(string.empty())
        return;

    size_t startIndex = 0;
    signBit = false;
    if(string[0] == '-')
    {
        ++startIndex;
        signBit = true;
    }
    else if(string[0] == '+')
    {
        ++startIndex;
    }

    size_t digitCount = string.size() - startIndex;
    size_t requiredWordCount = (digitCount * bitsPerDigitInRadix(radix) + 31) / 32;
    words.clear();
    words.reserve(requiredWordCount);
    for(size_t i = startIndex; i < string.size(); ++i)
    {
        auto digit = parseDigitInRadix(string[i], radix);
        if(digit >= 0)
            multiplyAndAddBySingleWordInPlace(*this, radix, uint32_t(digit));
    }

    normalize();
}

void LargeInteger::setUnnormalizedWords(const std::vector<uint32_t> &newWords)
{
    setUnnormalizedWords(newWords.data(), newWords.size());
}

void LargeInteger::setUnnormalizedWords(const uint32_t newWords[], size_t wordCount)
{
    auto normalizedWordCount = computeSignificantWordCount(newWords, wordCount);
    words.resize(normalizedWordCount);
    for(size_t i = 0; i < normalizedWordCount; ++i)
    {
        words[i] = newWords[i];
    }

    signBit = signBit && !words.empty();
}

void LargeInteger::normalize()
{
    words.resize(computeSignificantWordCount(words.data(), words.size()));

    signBit = signBit && !words.empty();
}

int32_t LargeInteger::compareWith(const LargeInteger &other) const
{
    auto mySign = sign();
    auto otherSign = other.sign();
    if(mySign != otherSign)
        return mySign - otherSign;
    else if(mySign == 0)
        return 0;

    auto result = compareMagnitudes(*this, other);
    return signBit ? -result : result;
}

bool LargeInteger::isZero() const
{
    return isAllZeroWords(words.data(), words.size());
}

bool LargeInteger::isOne() const
{
    return *this == One;
}

bool LargeInteger::isMinusOne() const
{
    return *this == MinusOne;
}

LargeInteger LargeInteger::operator-() const
{
    return LargeInteger{!signBit && !words.empty(), words};
}

LargeInteger LargeInteger::operator~() const
{
    return MinusOne - *this;
}

LargeInteger LargeInteger::operator+(const LargeInteger &other) const
{
    if(isZero())
        return other;
    else if(other.isZero())
        return *this;

    LargeInteger result;

    if(signBit == other.signBit)
    {
        // Addition.
        result.signBit = signBit;
        sumMagnitudesInto(*this, other, result);
    }
    else
    {
        // Subtraction.
        if(compareMagnitudes(*this, other) >= 0)
        {
            result.signBit = signBit;
            subtractMagnitudesInto(*this, other, result);
        }
        else
        {
            result.signBit = other.signBit;
            subtractMagnitudesInto(other, *this, result);
        }
    }

    result.normalize();
    return result;
}

LargeInteger &LargeInteger::operator+=(const LargeInteger &other)
{
    *this = *this + other;
    return *this;
}

LargeInteger LargeInteger::operator-(const LargeInteger &other) const
{
    if(isZero())
        return -other;

    LargeInteger result;

    if(signBit == other.signBit)
    {
        // Subtraction.
        if(compareMagnitudes(*this, other) >= 0)
        {
            result.signBit = signBit;
            subtractMagnitudesInto(*this, other, result);
        }
        else
        {
            result.signBit = other.signBit;
            subtractMagnitudesInto(other, *this, result);
        }
    }
    else
    {
        // Addition
        result.signBit = signBit;
        sumMagnitudesInto(*this, other, result);
    }

    result.normalize();
    return result;
}

LargeInteger &LargeInteger::operator-=(const LargeInteger &other)
{
    *this = *this - other;
    return *this;
}

LargeInteger LargeInteger::operator*(const LargeInteger &other) const
{
    // Fast special cases.
    if(other.isZero() || isZero())
        return Zero;
    else if(isOne())
        return other;
    else if(other.isOne())
        return *this;
    else if(isMinusOne() && other.isMinusOne())
        return One;
    else if(isMinusOne())
        return -other;
    else if(other.isMinusOne())
        return -(*this);

    LargeInteger result;
    result.signBit = signBit ^ other.signBit;
    if(words.size() >= other.words.size())
        multiplyMagnitudesInto(*this, other, result);
    else
        multiplyMagnitudesInto(other, *this, result);
    result.normalize();
    return result;
}

LargeInteger &LargeInteger::operator*=(const LargeInteger &other)
{
    *this = *this * other;
    return *this;
}

LargeInteger LargeInteger::operator/(const LargeInteger &divisor) const
{
    LargeInteger quotient;
    LargeInteger remainder;
    divisionAndRemainder(divisor, quotient, remainder);
    return quotient;
}

LargeInteger &LargeInteger::operator/=(const LargeInteger &divisor)
{
    *this = *this / divisor;
    return *this;
}

LargeInteger LargeInteger::operator%(const LargeInteger &divisor) const
{
    LargeInteger quotient;
    LargeInteger remainder;
    divisionAndRemainder(divisor, quotient, remainder);
    return remainder;
}

LargeInteger &LargeInteger::operator%=(const LargeInteger &divisor)
{
    *this = *this % divisor;
    return *this;
}

LargeInteger LargeInteger::operator<<(uint32_t shiftAmount) const
{
    if(isZero() || shiftAmount == 0)
        return *this;

    LargeInteger result;
    auto insertedWordCount = shiftAmount / 32;
    auto actualShiftedAmount = shiftAmount % 32;

    auto remainingFreeSpace = 32 - highBitOf(words.back());
    auto hasExtraWordAtEnd = actualShiftedAmount > remainingFreeSpace;
    auto resultSize = words.size() + insertedWordCount + (hasExtraWordAtEnd ? 1 : 0);
    result.signBit = signBit;
    result.words.resize(resultSize);

    uint32_t carry = 0;
    for(size_t i = 0; i < words.size(); ++i)
    {
        auto carriedWord = (uint64_t(words[i]) << actualShiftedAmount) | carry;
        result.words[i + insertedWordCount] = uint32_t(carriedWord & 0xFFFFFFFF);
        carry = carriedWord >> 32;
    }

    if(carry != 0)
    {
        assert(hasExtraWordAtEnd);
        result.words.back() = carry;
    }

    result.normalize();
    return result;
}

LargeInteger &LargeInteger::operator<<=(uint32_t shiftAmount)
{
    *this = *this << shiftAmount;
    return *this;
}

LargeInteger LargeInteger::operator>>(uint32_t shiftAmount) const
{
    if(isZero() || shiftAmount == 0)
        return *this;
    
    auto removedWordCount = shiftAmount / 32;
    if(removedWordCount >= words.size())
        return Zero;

    auto actualShiftedAmount = shiftAmount % 32;

    LargeInteger result;
    result.signBit = signBit;
    result.words.resize(words.size() - removedWordCount);
    if(actualShiftedAmount == 0)
    {
        for(size_t i = 0; i < result.words.size(); ++i)
            result.words[i] = words[i + removedWordCount];
    }
    else
    {
        uint32_t nextWordMask = (1<<actualShiftedAmount) - 1;
        uint32_t nextWordShift = 32 - actualShiftedAmount;
        for(size_t i = 0; i < result.words.size(); ++i)
        {
            uint32_t nextBits = 0;
            if(i + 1 < result.words.size())
            {
                auto nextWord = words[i + removedWordCount + 1];
                nextBits = (nextWord & nextWordMask) << nextWordShift;
            }
            result.words[i] = (words[i + removedWordCount] >> actualShiftedAmount) | nextBits;
        }
            
    }

    result.normalize();

    return result;
}

LargeInteger &LargeInteger::operator>>=(uint32_t shiftAmount)
{
    *this = *this >> shiftAmount;
    return *this;
}

LargeInteger LargeInteger::factorial() const
{
    if(isZero())
        return One;

    return *this * (*this + MinusOne).factorial();
}

LargeInteger LargeInteger::binomialCoefficient(const LargeInteger &n, const LargeInteger &k)
{
    return n.factorial() / (k.factorial()*(n - k).factorial());
}

void LargeInteger::divisionAndRemainder(const LargeInteger &divisor, LargeInteger &quotient, LargeInteger &remainder) const
{
    if(divisor.isZero())
        throw DivisionByZeroError();

    // Compare the magnitude to rule out the easy cases.
    auto magnitudeComparison = compareMagnitudes(*this, divisor);
    if(magnitudeComparison < 0)
    {
        quotient = Zero;
        remainder = *this;
        remainder.signBit = false;
        remainder.normalize();
        return;
    }
    else if(magnitudeComparison == 0)
    {
        if(signBit != divisor.signBit)
            quotient = MinusOne;
        else
            quotient = One;
        remainder = Zero;
        return;
    }

    auto lastWord = divisor.words.back();
    auto lastWordHighBit = highBitOf(lastWord);
    auto normalizationBitShift = 32 - lastWordHighBit;
    if(normalizationBitShift == 0)
    {
        divideMagnitudesInto(*this, divisor, quotient, remainder);
    }
    else
    {
        auto normalizedDividend = *this << normalizationBitShift;
        auto normalizedDivisor = divisor << normalizationBitShift;
        divideMagnitudesInto(normalizedDividend, normalizedDivisor, quotient, remainder);

        remainder >>= normalizationBitShift;
    }

    quotient.signBit = signBit ^ divisor.signBit;
    quotient.normalize();
    remainder.signBit = false;
    remainder.normalize();
}

std::string LargeInteger::asString() const
{
    if(isZero())
    {
        return "0";
    }

    LargeInteger currentValue = *this;
    LargeInteger quotient;
    LargeInteger remainder;
    std::string reverseResult;
    reverseResult.reserve(words.size()*32 / 4);
    while(!currentValue.isZero())
    {
        currentValue.divisionAndRemainder(Ten, quotient, remainder);
        reverseResult.push_back(remainder.wordAt(0) + '0');
        currentValue = quotient;
    }

    std::string result;
    result.reserve(reverseResult.size() + (signBit ? 1 : 0));
    if(signBit)
        result.push_back('-');
    result.insert(result.end(), reverseResult.rbegin(), reverseResult.rend());
    return result;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius