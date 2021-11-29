#include "sysmel/BootstrapEnvironment/SSACallInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSACallInstruction> SSACallInstructionTypeRegistration;

bool SSACallInstruction::isSSACallInstruction() const
{
    return true;
}

AnyValuePtr SSACallInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitCallInstruction(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius