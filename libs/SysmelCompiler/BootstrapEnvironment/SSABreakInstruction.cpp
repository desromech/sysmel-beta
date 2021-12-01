#include "sysmel/BootstrapEnvironment/SSABreakInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSABreakInstruction> SSABreakInstructionTypeRegistration;

bool SSABreakInstruction::isSSABreakInstruction() const
{
    return true;
}

bool SSABreakInstruction::isTerminatorInstruction() const
{
    return true;
}

AnyValuePtr SSABreakInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitBreakInstruction(selfFromThis());
}

TypePtr SSABreakInstruction::getValueType() const
{
    return Type::getVoidType();
}

std::string SSABreakInstruction::getMnemonic() const
{
    return "break";
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius