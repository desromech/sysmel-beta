#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_DISPATCH_MODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_DISPATCH_MODE_HPP
#pragma once

#include <stdint.h>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am the dispatch mode that is used for invoking a method.
 */
enum class MethodDispatchMode : uint8_t
{
    Default = 0,
    VirtualNew,
    VirtualOverride,
    Static,
};

inline const char *methodDispatchModeToString(MethodDispatchMode mode)
{
    switch(mode)
    {
    case MethodDispatchMode::VirtualNew: return "virtual";
    case MethodDispatchMode::VirtualOverride: return "override";
    case MethodDispatchMode::Static: return "static";
    case MethodDispatchMode::Default:
    default:
        return "default";
    }
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_DISPATCH_MODE_HPP