#ifndef SYSMEL_COMPILER_LITERAL_INTEGER_HPP
#define SYSMEL_COMPILER_LITERAL_INTEGER_HPP
#pragma once

#include <stdint.h>
#include <string>
#include <ostream>

namespace SysmelMoebius
{
namespace Compiler
{

/**
 * I represent a literal integer value.
 */
struct LiteralInteger
{
    int64_t value;

    LiteralInteger operator+() const
    {
        return LiteralInteger{value};
    }

    LiteralInteger operator-() const
    {
        return LiteralInteger{-value};
    }

    LiteralInteger operator~() const
    {
        return LiteralInteger{~value};
    }

    LiteralInteger operator+(const LiteralInteger &o) const
    {
        return LiteralInteger{value + o.value};
    }

    LiteralInteger operator-(const LiteralInteger &o) const
    {
        return LiteralInteger{value - o.value};
    }

    LiteralInteger operator*(const LiteralInteger &o) const
    {
        return LiteralInteger{value * o.value};
    }

    LiteralInteger operator/(const LiteralInteger &o) const
    {
        return LiteralInteger{value / o.value};
    }

    LiteralInteger operator%(const LiteralInteger &o) const
    {
        return LiteralInteger{value % o.value};
    }

    LiteralInteger operator^(const LiteralInteger &o) const
    {
        return LiteralInteger{value ^ o.value};
    }

    LiteralInteger operator|(const LiteralInteger &o) const
    {
        return LiteralInteger{value | o.value};
    }

    LiteralInteger operator&(const LiteralInteger &o) const
    {
        return LiteralInteger{value | o.value};
    }

    bool operator==(const LiteralInteger &o) const
    {
        return value == o.value;
    }

    bool operator!=(const LiteralInteger &o) const
    {
        return value != o.value;
    }

    bool operator<(const LiteralInteger &o) const
    {
        return value < o.value;
    }

    bool operator<=(const LiteralInteger &o) const
    {
        return value <= o.value;
    }

    bool operator>(const LiteralInteger &o) const
    {
        return value < o.value;
    }

    bool operator>=(const LiteralInteger &o) const
    {
        return value >= o.value;
    }

    friend std::ostream &operator<<(std::ostream &out, const LiteralInteger &literal)
    {
        return out << literal.value;
    }
};

} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_LITERAL_INTEGER_HPP
