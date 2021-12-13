#include "Environment/SSAGetAggregateFieldReferenceInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAGetAggregateFieldReferenceInstruction> SSAGetAggregateFieldReferenceInstructionTypeRegistration;

bool SSAGetAggregateFieldReferenceInstruction::isSSAGetAggregateFieldReferenceInstruction() const
{
    return true;
}

AnyValuePtr SSAGetAggregateFieldReferenceInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitGetAggregateFieldReferenceInstruction(selfFromThis());
}

std::string SSAGetAggregateFieldReferenceInstruction::getMnemonic() const
{
    return "getAggregateField";
}

void SSAGetAggregateFieldReferenceInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(aggregate);
}

void SSAGetAggregateFieldReferenceInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(aggregate);
}

TypePtr SSAGetAggregateFieldReferenceInstruction::getValueType() const
{
    return valueType;
}

void SSAGetAggregateFieldReferenceInstruction::setValueType(const TypePtr &type)
{
    valueType = type;
}

const SSAValuePtr &SSAGetAggregateFieldReferenceInstruction::getAggregate() const
{
    return aggregate;
}

void SSAGetAggregateFieldReferenceInstruction::setAggregate(const SSAValuePtr &newAggregate)
{
    aggregate = newAggregate;
    if(aggregate)
        aggregate->addUse(selfFromThis());
}

const FieldVariablePtr &SSAGetAggregateFieldReferenceInstruction::getFieldVariable() const
{
    return fieldVariable;
}

void SSAGetAggregateFieldReferenceInstruction::setFieldVariable(const FieldVariablePtr &newFieldVariable)
{
    fieldVariable = newFieldVariable;
}

} // End of namespace Environment
} // End of namespace Sysmel