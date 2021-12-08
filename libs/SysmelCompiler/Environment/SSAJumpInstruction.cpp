#include "Environment/SSAJumpInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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
} // End of namespace Environment
} // End of namespace Sysmel