#include "Environment/RuntimeContextTargetDescription.hpp"

namespace Sysmel
{
namespace Environment
{

RuntimeContextTargetDescription RuntimeContextTargetDescription::makeForScripting()
{
    RuntimeContextTargetDescription target;
    target.targetMachineKind = TargetMachineKind::Scripting;
    return target;
}

RuntimeContextTargetDescription RuntimeContextTargetDescription::makeForHost()
{
    RuntimeContextTargetDescription target;
    target.targetMachineKind = TargetMachineKind::CPU;
    target.pointerSize = sizeof(void*);
    target.pointerAlignment = alignof(void*);
    target.doubleAlignment = alignof(double);
    return target;
}

RuntimeContextTargetDescription RuntimeContextTargetDescription::makeForCPU32()
{
    RuntimeContextTargetDescription target;
    target.targetMachineKind = TargetMachineKind::CPU;
    target.pointerSize = 4;
    target.pointerAlignment = 4;
    target.doubleAlignment = 4;
    return target;
}

RuntimeContextTargetDescription RuntimeContextTargetDescription::makeForCPU64()
{
    RuntimeContextTargetDescription target;
    target.targetMachineKind = TargetMachineKind::CPU;
    target.pointerSize = 8;
    target.pointerAlignment = 8;
    target.doubleAlignment = 8;
    return target;
}

} // End of namespace Environment
} // End of namespace Sysmel