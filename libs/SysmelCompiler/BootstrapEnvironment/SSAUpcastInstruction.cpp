#include "sysmel/BootstrapEnvironment/SSAUpcastInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAUpcastInstruction> SSAUpcastInstructionTypeRegistration;

bool SSAUpcastInstruction::isSSAUpcastInstruction() const
{
    return true;
}

AnyValuePtr SSAUpcastInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitUpcastInstruction(selfFromThis());
}

std::string SSAUpcastInstruction::getMnemonic() const
{
    return "upcast";
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius