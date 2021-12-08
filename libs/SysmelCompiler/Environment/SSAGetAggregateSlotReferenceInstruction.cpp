#include "Environment/SSAGetAggregateSlotReferenceInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAGetAggregateSlotReferenceInstruction> SSAGetAggregateSlotReferenceInstructionTypeRegistration;

bool SSAGetAggregateSlotReferenceInstruction::isSSAGetAggregateSlotReferenceInstruction() const
{
    return true;
}

AnyValuePtr SSAGetAggregateSlotReferenceInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitGetAggregateSlotReferenceInstruction(selfFromThis());
}

std::string SSAGetAggregateSlotReferenceInstruction::getMnemonic() const
{
    return "getAggregateSlot";
}

void SSAGetAggregateSlotReferenceInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(aggregate);
    aBlock(slotIndex);
}

void SSAGetAggregateSlotReferenceInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(aggregate);
    aBlock(slotIndex);
}

TypePtr SSAGetAggregateSlotReferenceInstruction::getValueType() const
{
    return valueType;
}

void SSAGetAggregateSlotReferenceInstruction::setValueType(const TypePtr &type)
{
    valueType = type;
}

const SSAValuePtr &SSAGetAggregateSlotReferenceInstruction::getAggregate() const
{
    return aggregate;
}

void SSAGetAggregateSlotReferenceInstruction::setAggregate(const SSAValuePtr &newAggregate)
{
    aggregate = newAggregate;
}

const SSAValuePtr &SSAGetAggregateSlotReferenceInstruction::getSlotIndex() const
{
    return slotIndex;
}

void SSAGetAggregateSlotReferenceInstruction::setSlotIndex(const SSAValuePtr &newSlotIndex)
{
    slotIndex = newSlotIndex;
}

} // End of namespace Environment
} // End of namespace Sysmel