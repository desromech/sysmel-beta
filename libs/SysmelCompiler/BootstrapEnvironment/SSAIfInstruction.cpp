#include "sysmel/BootstrapEnvironment/SSAIfInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAIfInstruction> SSAIfInstructionTypeRegistration;

bool SSAIfInstruction::isSSAIfInstruction() const
{
    return true;
}

AnyValuePtr SSAIfInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitIfInstruction(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius