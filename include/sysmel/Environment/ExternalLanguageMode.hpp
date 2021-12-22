#ifndef SYSMEL_ENVIRONMENT_EXTERNAL_LANGUAGE_MODE_HPP
#define SYSMEL_ENVIRONMENT_EXTERNAL_LANGUAGE_MODE_HPP
#pragma once

#include <stdint.h>

namespace Sysmel
{
namespace Environment
{

/**
 * I am an a external language mode.
 */
enum class ExternalLanguageMode : uint8_t
{
    None = 0,
    C,
    Cpp,
};

inline const char *externalLanguageModeToString(ExternalLanguageMode visibility)
{
    switch(visibility)
    {
    case ExternalLanguageMode::C: return "C";
    case ExternalLanguageMode::Cpp: return "Cpp";
    case ExternalLanguageMode::None:
    default: return "none";
    }
}


} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_EXTERNAL_LANGUAGE_MODE_HPP