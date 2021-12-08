#include "Environment/SSAConditionalJumpInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAConditionalJumpInstruction> SSAConditionalJumpInstructionTypeRegistration;

bool SSAConditionalJumpInstruction::isSSAConditionalJumpInstruction() const
{
    return true;
}

bool SSAConditionalJumpInstruction::isTerminatorInstruction() const
{
    return true;
}

AnyValuePtr SSAConditionalJumpInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitConditionalJumpInstruction(selfFromThis());
}

TypePtr SSAConditionalJumpInstruction::getValueType() const
{
    return Type::getVoidType();
}

} // End of namespace Environment
} // End of namespace Sysmel