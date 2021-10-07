#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_VISIBILITY_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_VISIBILITY_HPP
#pragma once

#include <stdint.h>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a specific visibility for a program entity.
 */
enum class ProgramEntityVisibility : uint8_t
{
    Default = 0,
    Private,
    Public,
    Internal,
    LexicalScope,
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_VISIBILITY_HPP