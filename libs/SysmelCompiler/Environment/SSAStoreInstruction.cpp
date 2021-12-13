#include "Environment/SSAStoreInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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
    if(value)
        value->addUse(selfFromThis());
}

const SSAValuePtr &SSAStoreInstruction::getReference() const
{
    return reference;
}

void SSAStoreInstruction::setReference(const SSAValuePtr &newReference)
{
    reference = newReference;
    if(reference)
        reference->addUse(selfFromThis());
}

} // End of namespace Environment
} // End of namespace Sysmel