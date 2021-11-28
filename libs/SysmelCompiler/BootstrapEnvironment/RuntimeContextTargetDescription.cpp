#include "sysmel/BootstrapEnvironment/RuntimeContextTargetDescription.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
    return target;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius