#include "sysmel/BootstrapEnvironment/SSADoWithCleanupInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSADoWithCleanupInstruction> SSADoWithCleanupInstructionTypeRegistration;

bool SSADoWithCleanupInstruction::isSSADoWithCleanupInstruction() const
{
    return true;
}

AnyValuePtr SSADoWithCleanupInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitDoWithCleanupInstruction(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius