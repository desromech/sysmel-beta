#ifndef SYSMEL_ENVIRONMENT_DLL_LINKAGE_MODE_HPP
#define SYSMEL_ENVIRONMENT_DLL_LINKAGE_MODE_HPP
#pragma once

#include <stdint.h>

namespace Sysmel
{
namespace Environment
{

/**
 * I am a dll linkage mode.
 */
enum class DllLinkageMode : uint8_t
{
    None = 0,
    Export,
    Import,
};

inline const char *dllLinkageModeToString(DllLinkageMode visibility)
{
    switch(visibility)
    {
    case DllLinkageMode::Export: return "export";
    case DllLinkageMode::Import: return "import";
    case DllLinkageMode::None:
    default: return "none";
    }
}


} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_DLL_LINKAGE_MODE_HPP