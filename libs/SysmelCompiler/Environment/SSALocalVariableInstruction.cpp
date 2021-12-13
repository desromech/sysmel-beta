#include "Environment/SSALocalVariableInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSALocalVariableInstruction> SSALocalVariableInstructionTypeRegistration;

bool SSALocalVariableInstruction::isSSALocalVariableInstruction() const
{
    return true;
}

AnyValuePtr SSALocalVariableInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitLocalVariableInstruction(selfFromThis());
}

TypePtr SSALocalVariableInstruction::getValueType() const
{
    return referenceType;
}

std::string SSALocalVariableInstruction::getMnemonic() const
{
    return "localVariable";
}

const TypePtr &SSALocalVariableInstruction::getVariableValueType() const
{
    return variableValueType;
}

void SSALocalVariableInstruction::setVariableValueType(const TypePtr &newValueType)
{
    variableValueType = newValueType;
}

const TypePtr &SSALocalVariableInstruction::getVariableReferenceType() const
{
    return referenceType;
}

void SSALocalVariableInstruction::setVariableReferenceType(const TypePtr &newReferenceType)
{
    referenceType = newReferenceType;
}

} // End of namespace Environment
} // End of namespace Sysmel