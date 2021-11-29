#include "sysmel/BootstrapEnvironment/SSAMakeClosureInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAMakeClosureInstruction> SSAMakeClosureInstructionTypeRegistration;

bool SSAMakeClosureInstruction::isSSAMakeClosureInstruction() const
{
    return true;
}

AnyValuePtr SSAMakeClosureInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitMakeClosureInstruction(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius