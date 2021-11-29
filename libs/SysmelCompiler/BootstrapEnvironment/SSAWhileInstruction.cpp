#include "sysmel/BootstrapEnvironment/SSAWhileInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAWhileInstruction> SSAWhileInstructionTypeRegistration;

bool SSAWhileInstruction::isSSAWhileInstruction() const
{
    return true;
}

AnyValuePtr SSAWhileInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitWhileInstruction(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius