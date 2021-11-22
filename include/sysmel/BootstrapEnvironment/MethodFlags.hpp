#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_FLAGS_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_FLAGS_HPP
#pragma once

#include <stdint.h>

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
    Constructor = 1<<1,
    Explicit = 1<<2,
    Pure = 1<<3,
    Abstract = 1<<4,
    Macro = 1<<5,
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_METHOD_FLAGS_HPP