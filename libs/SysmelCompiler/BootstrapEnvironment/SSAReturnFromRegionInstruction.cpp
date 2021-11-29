#include "sysmel/BootstrapEnvironment/SSAReturnFromRegionInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAReturnFromRegionInstruction> SSAReturnFromRegionInstructionTypeRegistration;

bool SSAReturnFromRegionInstruction::isSSAReturnFromRegionInstruction() const
{
    return true;
}

AnyValuePtr SSAReturnFromRegionInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitReturnFromRegionInstruction(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius