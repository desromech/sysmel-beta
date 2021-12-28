#ifndef SYSMEL_ENVIRONMENT_METHOD_FLAGS_HPP
#define SYSMEL_ENVIRONMENT_METHOD_FLAGS_HPP
#pragma once

#include <stdint.h>

#include "SExpression.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * Flags for a method
 */
enum class MethodFlags : uint32_t
{
    None = 0,

    // Kind
    Macro = 1<<0,
    Fallback = 1<<1,
    MessageMethod =1<<2,

    // Side effects
    Const = 1<<3,
    Pure = 1<<4,

    // Unwinding semantics
    NoThrow = 1<<5,
    ReturnsTwice = 1<<6,

    // Type conversions
    Constructor = 1<<7,
    Conversion = 1<<8,
    Explicit = 1<<9,

    // Dispatch mode
    Abstract = 1<<10,
    Final = 1<<11,
    Override = 1<<12,
    Virtual = 1<<13,
    Static = 1<<14,

    // Special semantics.
    Trivial = 1<<15,
};

inline MethodFlags operator|(MethodFlags a, MethodFlags b)
{
    return MethodFlags(uint32_t(a) | uint32_t(b));
}

inline MethodFlags operator&(MethodFlags a, MethodFlags b)
{
    return MethodFlags(uint32_t(a) & uint32_t(b));
}

inline MethodFlags operator~(MethodFlags a)
{
    return MethodFlags(~uint32_t(a));
}

SYSMEL_COMPILER_LIB_EXPORT SExpression methodFlagsToSExpression(MethodFlags flags);

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_METHOD_FLAGS_HPP