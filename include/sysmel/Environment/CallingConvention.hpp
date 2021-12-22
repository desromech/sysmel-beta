#ifndef SYSMEL_ENVIRONMENT_CALLING_CONVENTION_HPP
#define SYSMEL_ENVIRONMENT_CALLING_CONVENTION_HPP
#pragma once

#include <stdint.h>

namespace Sysmel
{
namespace Environment
{

/**
 * I am a calling convention name.
 */
enum class CallingConvention : uint8_t
{
    Default = 0,
    Cdecl,
    Stdcall,
    Thiscall,
    Apicall,
};

inline const char *callingConventionToString(CallingConvention visibility)
{
    switch(visibility)
    {
    case CallingConvention::Cdecl: return "cdecl";
    case CallingConvention::Stdcall: return "stdcall";
    case CallingConvention::Thiscall: return "thiscall";
    case CallingConvention::Apicall: return "apicall";
    case CallingConvention::Default:
    default: return "default";
    }
}


} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_CALLING_CONVENTION_HPP