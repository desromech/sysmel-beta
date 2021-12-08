#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_VISIBILITY_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_VISIBILITY_HPP
#pragma once

#include <stdint.h>

namespace Sysmel
{
namespace Environment
{

/**
 * I am a specific visibility for a program entity.
 */
enum class ProgramEntityVisibility : uint8_t
{
    Default = 0,
    Private,
    Public,
    Protected,
    Internal,
    LexicalScope,
};

inline const char *programEntityVisibilityToString(ProgramEntityVisibility visibility)
{
    switch(visibility)
    {
    case ProgramEntityVisibility::Private: return "private";
    case ProgramEntityVisibility::Public: return "public";
    case ProgramEntityVisibility::Internal: return "internal";
    case ProgramEntityVisibility::LexicalScope: return "lexical";
    case ProgramEntityVisibility::Default:
    default: return "default";
    }
}


} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_VISIBILITY_HPP