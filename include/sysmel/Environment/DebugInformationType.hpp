#ifndef SYSMEL_ENVIRONMENT_DEBUG_INFORMATION_TYPE_HPP
#define SYSMEL_ENVIRONMENT_DEBUG_INFORMATION_TYPE_HPP
#pragma once

#include <stdint.h>

namespace Sysmel
{
namespace Environment
{

/**
 * I am a specific visibility for a program entity.
 */
enum class DebugInformationType : uint8_t
{
    None = 0,
    Default,
    Dwarf,
    Dwarf2,
    Dwarf3,
    Dwarf4,
    CodeView,
};

inline const char *debugInformationTypeToString(DebugInformationType type)
{
    switch(type)
    {
    case DebugInformationType::None:
    case DebugInformationType::Default: return "default";
    case DebugInformationType::Dwarf: return "dwarf";
    case DebugInformationType::Dwarf2: return "dwarf2";
    case DebugInformationType::Dwarf3: return "dwarf3";
    case DebugInformationType::Dwarf4: return "dwarf4";
    case DebugInformationType::CodeView: return "codeView";
    default: return "none";
    }
}


} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_DEBUG_INFORMATION_TYPE_HPP