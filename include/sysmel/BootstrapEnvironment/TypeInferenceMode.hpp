#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_INFERENCE_MODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_INFERENCE_MODE_HPP
#pragma once

#include <stdint.h>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a specific visibility for a program entity.
 */
enum class TypeInferenceMode : uint8_t
{
    Value = 0,
    Reference,
    TemporaryReference
};

inline const char *typeInferenceModeToString(TypeInferenceMode kind)
{
    switch(kind)
    {
    case TypeInferenceMode::Reference: return "ref";
    case TypeInferenceMode::TemporaryReference: return "tempRef";
    case TypeInferenceMode::Value:
    default: return "value";
    }
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_INFERENCE_MODE_HPP