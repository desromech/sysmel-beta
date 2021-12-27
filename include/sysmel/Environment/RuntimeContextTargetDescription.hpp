#ifndef SYSMEL_ENVIRONMENT_RUNTIME_CONTEXT_TARGET_DESCRIPTION_HPP
#define SYSMEL_ENVIRONMENT_RUNTIME_CONTEXT_TARGET_DESCRIPTION_HPP
#pragma once

#include <string>

namespace Sysmel
{
namespace Environment
{

enum class TargetMachineKind
{
    Scripting = 0,
    CPU,
    GPU,
    AI,
    FPGA,
};

std::string getHostTargetTriple();

/**
 * I describe a target name and features.
 */
struct TargetNameAndFeatures
{
    static TargetNameAndFeatures getForHost();
    static TargetNameAndFeatures getForCPU32();
    static TargetNameAndFeatures getForCPU64();

    static TargetNameAndFeatures forTriple(const std::string &triple);

    void setTriple(const std::string &newTriple);

    bool isDarwin() const;
    bool isLinux() const;
    bool isWindows() const;

    bool isARM() const;
    bool isARM64() const;
    bool isX86() const;
    bool isX86_64() const;

    bool defaultsToPIE() const;

    std::string triple;
    std::string architecture;
    std::string subarchitecture;
    std::string vendor;
    std::string system;
    std::string abi;
    std::string cpu = "generic";
    std::string fpu;
    std::string floatAbi;
};

/**
 * I describe a specific compilation target that corresponds to a runtime context.
 */
struct RuntimeContextTargetDescription
{
    static RuntimeContextTargetDescription makeForScripting();
    static RuntimeContextTargetDescription makeForHost();
    static RuntimeContextTargetDescription makeForCPU32();
    static RuntimeContextTargetDescription makeForCPU64();
    static RuntimeContextTargetDescription makeForTargetNameAndFeatures(const TargetNameAndFeatures &targetNameAndFeatures);

    TargetMachineKind targetMachineKind = TargetMachineKind::CPU;

    TargetNameAndFeatures nameAndFeatures;
    
    uint32_t pointerSize = 8;
    uint32_t pointerAlignment = 8;

    uint32_t doubleAlignment = 8;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_RUNTIME_CONTEXT_TARGET_DESCRIPTION_HPP