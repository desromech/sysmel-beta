#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_DECORATIONS_FLAGS_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_DECORATIONS_FLAGS_HPP
#pragma once

#include <stdint.h>

namespace Sysmel
{
namespace Environment
{

/**
 * Flags for a method
 */
enum class TypeDecorationFlags : uint32_t
{
    None = 0,
    Const = 1<<1,
    Volatile = 1<<2,
    Restrict = 1<<3,
};

inline TypeDecorationFlags operator|(TypeDecorationFlags a, TypeDecorationFlags b)
{
    return TypeDecorationFlags(uint32_t(a) | uint32_t(b));
}

inline TypeDecorationFlags operator&(TypeDecorationFlags a, TypeDecorationFlags b)
{
    return TypeDecorationFlags(uint32_t(a) & uint32_t(b));
}

inline TypeDecorationFlags operator~(TypeDecorationFlags a)
{
    return TypeDecorationFlags(~uint32_t(a));
}

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_DECORATIONS_FLAGS_HPP