#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_FLAGS_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_FLAGS_HPP
#pragma once

#include <stdint.h>

#include "SExpression.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * Flags for a method
 */
enum class MethodFlags : uint32_t
{
    None = 0,

    // Kind
    Macro = 1<<0,
    MessageMethod =1<<1,

    // Side effects
    Const = 1<<2,
    Pure = 1<<3,

    // Unwinding semantics
    NoThrow = 1<<4,
    ReturnsTwice = 1<<5,

    // Type conversions
    Constructor = 1<<6,
    Conversion = 1<<7,
    Explicit = 1<<8,

    // Dispatch mode
    Abstract = 1<<9,
    Final = 1<<10,
    Override = 1<<11,
    Virtual = 1<<12,
    Static = 1<<13,
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

SExpression methodFlagsToSExpression(MethodFlags flags);

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_FLAGS_HPP