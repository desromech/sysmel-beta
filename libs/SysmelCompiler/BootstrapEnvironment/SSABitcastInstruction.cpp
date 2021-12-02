#include "sysmel/BootstrapEnvironment/SSABitcastInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSABitcastInstruction> SSABitcastInstructionTypeRegistration;

bool SSABitcastInstruction::isSSABitcastInstruction() const
{
    return true;
}

AnyValuePtr SSABitcastInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitBitcastInstruction(selfFromThis());
}

std::string SSABitcastInstruction::getMnemonic() const
{
    return "upcast";
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius