#include "Environment/SSATrapInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSATrapInstruction> SSATrapInstructionTypeRegistration;

bool SSATrapInstruction::isSSATrapInstruction() const
{
    return true;
}

bool SSATrapInstruction::isTerminatorInstruction() const
{
    return true;
}

AnyValuePtr SSATrapInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitTrapInstruction(selfFromThis());
}

TypePtr SSATrapInstruction::getValueType() const
{
    return Type::getVoidType();
}

std::string SSATrapInstruction::getMnemonic() const
{
    return "trap";
}

void SSATrapInstruction::setReason(TrapReason newReason)
{
    reason = newReason;
}

TrapReason SSATrapInstruction::getReason() const
{
    return reason;
}

} // End of namespace Environment
} // End of namespace Sysmel