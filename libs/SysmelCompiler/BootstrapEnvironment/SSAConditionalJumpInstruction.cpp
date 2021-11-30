#include "sysmel/BootstrapEnvironment/SSAConditionalJumpInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAConditionalJumpInstruction> SSAConditionalJumpInstructionTypeRegistration;

bool SSAConditionalJumpInstruction::isSSAConditionalJumpInstruction() const
{
    return true;
}

bool SSAConditionalJumpInstruction::isTerminatorInstruction() const
{
    return true;
}

AnyValuePtr SSAConditionalJumpInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitConditionalJumpInstruction(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius