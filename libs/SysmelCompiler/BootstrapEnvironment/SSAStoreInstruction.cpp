#include "sysmel/BootstrapEnvironment/SSAStoreInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAStoreInstruction> SSAStoreInstructionTypeRegistration;

bool SSAStoreInstruction::isSSAStoreInstruction() const
{
    return true;
}

AnyValuePtr SSAStoreInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitStoreInstruction(selfFromThis());
}

TypePtr SSAStoreInstruction::getValueType() const
{
    return Type::getVoidType();
}

std::string SSAStoreInstruction::getMnemonic() const
{
    return "store";
}

void SSAStoreInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(value);
    aBlock(reference);
}

void SSAStoreInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(value);
    aBlock(reference);
}

const SSAValuePtr &SSAStoreInstruction::getValue() const
{
    return value;
}

void SSAStoreInstruction::setValue(const SSAValuePtr &newValue)
{
    value = newValue;
}

const SSAValuePtr &SSAStoreInstruction::getReference() const
{
    return reference;
}

void SSAStoreInstruction::setReference(const SSAValuePtr &newReference)
{
    reference = newReference;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius