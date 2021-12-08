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
    return target;
}

} // End of namespace Environment
} // End of namespace Sysmel