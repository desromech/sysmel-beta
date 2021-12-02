#include "sysmel/BootstrapEnvironment/SSADowncastInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSADowncastInstruction> SSADowncastInstructionTypeRegistration;

bool SSADowncastInstruction::isSSADowncastInstruction() const
{
    return true;
}

AnyValuePtr SSADowncastInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitDowncastInstruction(selfFromThis());
}

std::string SSADowncastInstruction::getMnemonic() const
{
    return "upcast";
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius