#include "sysmel/BootstrapEnvironment/SSAMakeAggregateInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAMakeAggregateInstruction> SSAMakeAggregateInstructionTypeRegistration;

bool SSAMakeAggregateInstruction::isSSAMakeAggregateInstruction() const
{
    return true;
}

AnyValuePtr SSAMakeAggregateInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitMakeAggregateInstruction(selfFromThis());
}

std::string SSAMakeAggregateInstruction::getMnemonic() const
{
    return "makeAggregate";
}

void SSAMakeAggregateInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    for(auto &el : elements)
        aBlock(el);
}

void SSAMakeAggregateInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    for(auto &el : elements)
        aBlock(el);
}

TypePtr SSAMakeAggregateInstruction::getValueType() const
{
    return valueType;
}

void SSAMakeAggregateInstruction::setValueType(const TypePtr &type)
{
    valueType = type;
}

const SSAValuePtrList &SSAMakeAggregateInstruction::getElements() const
{
    return elements;
}

void SSAMakeAggregateInstruction::setElements(const SSAValuePtrList &newElements)
{
    elements = newElements;
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius