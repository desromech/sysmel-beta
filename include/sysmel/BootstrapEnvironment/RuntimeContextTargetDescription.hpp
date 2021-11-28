#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RUNTIME_CONTEXT_TARGET_DESCRIPTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RUNTIME_CONTEXT_TARGET_DESCRIPTION_HPP
#pragma once

#include <string>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

enum class TargetMachineKind
{
    Scripting = 0,
    CPU,
    GPU,
    AI,
    FPGA,
};

/**
 * I describe a specific compilation target that corresponds to a runtime context.
 */
struct RuntimeContextTargetDescription
{
    static RuntimeContextTargetDescription makeForScripting();
    static RuntimeContextTargetDescription makeForHost();

    TargetMachineKind targetMachineKind = TargetMachineKind::CPU;
    
    uint32_t pointerSize = 8;
    uint32_t pointerAlignment = 8;

    uint32_t doubleAlignment = 8;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RUNTIME_CONTEXT_TARGET_DESCRIPTION_HPP