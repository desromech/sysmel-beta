#include "sysmel/BootstrapEnvironment/SSALocalVariableInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
    return "load";
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius