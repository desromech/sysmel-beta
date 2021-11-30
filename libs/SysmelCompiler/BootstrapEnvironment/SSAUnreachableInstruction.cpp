#include "sysmel/BootstrapEnvironment/SSAUnreachableInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAUnreachableInstruction> SSAUnreachableInstructionTypeRegistration;

bool SSAUnreachableInstruction::isSSAUnreachableInstruction() const
{
    return true;
}

bool SSAUnreachableInstruction::isTerminatorInstruction() const
{
    return true;
}

AnyValuePtr SSAUnreachableInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitUnreachableInstruction(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius