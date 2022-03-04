#include "Environment/RuntimeContextTargetDescription.hpp"
#include "Environment/StringUtilities.hpp"
#include <unordered_map>

namespace Sysmel
{
namespace Environment
{

#ifndef BUILD_WITH_LLVM_BACKEND
std::string getHostTargetTriple()
{
    std::string result;

    // Architecture
#if defined(__i386__)
    result += "unknown";
#elif defined(__x86_64__)
    result += "x86_64";
#elif defined(__arm64__)
    result += "arm64";
#else
#endif

    // Vendor
#if defined(__APPLE__)
    result += "-apple";
#elif defined(__i386__) || defined(__x86_64__)
    result += "-pc";
#else
    result += "-unknown";
#endif

    // OS
#if defined(__linux__)
    result += "-linux";
#elif defined(_WIN32)
    result += "-win32";
#elif defined(__APPLE__)
    result += "-darwin";
#else
    result += "-unknown";
#endif

// ABI
#if defined(__linux__)
    result += "-gnu";
#elif defined(_WIN32)
    result += "-msvc";
#elif defined(__APPLE__)
    // Not required
#else
    result += "-unknown";
#endif
    return result;
}
#endif

static std::unordered_map<std::string, std::pair<TargetMachineKind, uint32_t>> architectureDatabase = {
    {"i386", {TargetMachineKind::CPU, 4}},
    {"x86_64", {TargetMachineKind::CPU, 8}},
    {"arm", {TargetMachineKind::CPU, 4}},
    {"arm64", {TargetMachineKind::CPU, 8}},
    {"mips", {TargetMachineKind::CPU, 4}},
    {"mipsel", {TargetMachineKind::CPU, 4}},
    {"wasm32", {TargetMachineKind::CPU, 4}},
    {"wasm64", {TargetMachineKind::CPU, 8}},
    {"spvl32", {TargetMachineKind::GPU, 4}},
    {"spvl64", {TargetMachineKind::GPU, 8}},
    {"spv32", {TargetMachineKind::GPU, 4}},
    {"spv64", {TargetMachineKind::GPU, 8}},
};

void TargetNameAndFeatures::setTriple(const std::string &newTriple)
{
    triple = newTriple;
    
    auto tripleParts = split(triple, '-');
    auto partCount = tripleParts.size();
    if(partCount >= 1)
        architecture = tripleParts[0];
    if(partCount >= 2)
        vendor = tripleParts[1];
    if(partCount >= 3)
        system = tripleParts[2];
    if(partCount >= 4)
        abi = tripleParts[3];

    // ARM subarchitecture
    if(stringBeginsWith(architecture, "arm"))
    {
        if(stringBeginsWith(architecture, "arm64"))
        {
            subarchitecture = architecture.substr(5);
            architecture = "arm64";
        }
        else
        {
            subarchitecture = architecture.substr(3);
            architecture = "arm";
        }
    }
}

TargetNameAndFeatures TargetNameAndFeatures::getForHost()
{
    return forTriple(getHostTargetTriple());
}

TargetNameAndFeatures TargetNameAndFeatures::getForCPU32()
{
    return forTriple("cpu64-sysmel-sysmel-sysmel");
}

TargetNameAndFeatures TargetNameAndFeatures::getForCPU64()
{
    return forTriple("cpu64-sysmel-sysmel-sysmel");
}

TargetNameAndFeatures TargetNameAndFeatures::forTriple(const std::string &triple)
{
    TargetNameAndFeatures result;
    result.setTriple(triple);
    return result;
}

bool TargetNameAndFeatures::isDarwin() const
{
    return stringBeginsWith(system, "darwin");
}

bool TargetNameAndFeatures::isLinux() const
{
    return system == "linux";
}

bool TargetNameAndFeatures::isWindows() const
{
    return system == "windows";
}

bool TargetNameAndFeatures::isARM() const
{
    return architecture == "arm";
}

bool TargetNameAndFeatures::isARM64() const
{
    return architecture == "arm64";
}

bool TargetNameAndFeatures::isX86() const
{
    return architecture == "i386";
}

bool TargetNameAndFeatures::isX86_64() const
{
    return architecture == "x86_64";
}

bool TargetNameAndFeatures::defaultsToPIE() const
{
    return isLinux() || isDarwin();
}

RuntimeContextTargetDescription RuntimeContextTargetDescription::makeForHost()
{
    RuntimeContextTargetDescription target;
    target.targetMachineKind = TargetMachineKind::CPU;
    target.nameAndFeatures = TargetNameAndFeatures::getForHost();
    target.pointerSize = sizeof(void*);
    target.pointerAlignment = alignof(void*);
    target.doubleAlignment = alignof(double);
    return target;
}

RuntimeContextTargetDescription RuntimeContextTargetDescription::makeForScripting()
{
    RuntimeContextTargetDescription target;
    target.targetMachineKind = TargetMachineKind::Scripting;
    return target;
}

RuntimeContextTargetDescription RuntimeContextTargetDescription::makeForCPU32()
{
    RuntimeContextTargetDescription target;
    target.targetMachineKind = TargetMachineKind::CPU;
    target.nameAndFeatures = TargetNameAndFeatures::getForCPU32();
    target.pointerSize = 4;
    target.pointerAlignment = 4;
    target.doubleAlignment = 4;
    return target;
}

RuntimeContextTargetDescription RuntimeContextTargetDescription::makeForCPU64()
{
    RuntimeContextTargetDescription target;
    target.targetMachineKind = TargetMachineKind::CPU;
    target.nameAndFeatures = TargetNameAndFeatures::getForCPU64();
    target.pointerSize = 8;
    target.pointerAlignment = 8;
    target.doubleAlignment = 8;
    return target;
}

RuntimeContextTargetDescription RuntimeContextTargetDescription::makeForTargetNameAndFeatures(const TargetNameAndFeatures &targetNameAndFeatures)
{
    RuntimeContextTargetDescription target;
    target.targetMachineKind = TargetMachineKind::CPU;
    target.nameAndFeatures = targetNameAndFeatures;
    auto pointerSize = 8;

    // Find the architecture on the database.
    {
        auto it = architectureDatabase.find(targetNameAndFeatures.architecture);
        if(it != architectureDatabase.end())
        {
            target.targetMachineKind = it->second.first;
            pointerSize = it->second.second;
        }
    }

    target.pointerSize = pointerSize;
    target.pointerAlignment = pointerSize;
    
    target.doubleAlignment = pointerSize;
    // On x86_32 windows double has an alignment of 4.
    if(targetNameAndFeatures.isWindows() && targetNameAndFeatures.isX86())
        target.doubleAlignment = 4;

    return target;
}

} // End of namespace Environment
} // End of namespace Sysmel