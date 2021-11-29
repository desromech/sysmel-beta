#include "sysmel/BootstrapEnvironment/SSADoWhileInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSADoWhileInstruction> SSADoWhileInstructionTypeRegistration;

bool SSADoWhileInstruction::isSSADoWhileInstruction() const
{
    return true;
}

AnyValuePtr SSADoWhileInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitDoWhileInstruction(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius