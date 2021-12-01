#include "sysmel/BootstrapEnvironment/SSAJumpInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAJumpInstruction> SSAJumpInstructionTypeRegistration;

bool SSAJumpInstruction::isSSAJumpInstruction() const
{
    return true;
}

bool SSAJumpInstruction::isTerminatorInstruction() const
{
    return true;
}

AnyValuePtr SSAJumpInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitJumpInstruction(selfFromThis());
}

TypePtr SSAJumpInstruction::getValueType() const
{
    return Type::getVoidType();
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius