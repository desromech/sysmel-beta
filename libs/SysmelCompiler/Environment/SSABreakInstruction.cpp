#include "Environment/SSABreakInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel