#include "sysmel/BootstrapEnvironment/SSALoadInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSALoadInstruction> SSALoadInstructionTypeRegistration;

bool SSALoadInstruction::isSSALoadInstruction() const
{
    return true;
}

AnyValuePtr SSALoadInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitLoadInstruction(selfFromThis());
}

TypePtr SSALoadInstruction::getValueType() const
{
    return valueType;
}

void SSALoadInstruction::setValueType(const TypePtr &newValueType)
{
    valueType = newValueType;
}

std::string SSALoadInstruction::getMnemonic() const
{
    return "load";
}

void SSALoadInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(reference);
}

void SSALoadInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(reference);
}

const SSAValuePtr &SSALoadInstruction::getReference() const
{
    return reference;
}

void SSALoadInstruction::setReference(const SSAValuePtr &newReference)
{
    reference = newReference;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius