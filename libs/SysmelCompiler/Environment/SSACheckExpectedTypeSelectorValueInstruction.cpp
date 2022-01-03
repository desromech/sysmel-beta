#include "Environment/SSACheckExpectedTypeSelectorValueInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSACheckExpectedTypeSelectorValueInstruction> SSACheckExpectedTypeSelectorValueInstructionTypeRegistration;

bool SSACheckExpectedTypeSelectorValueInstruction::isSSACheckExpectedTypeSelectorValueInstruction() const
{
    return true;
}

AnyValuePtr SSACheckExpectedTypeSelectorValueInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitCheckExpectedTypeSelectorValueInstruction(selfFromThis());
}

TypePtr SSACheckExpectedTypeSelectorValueInstruction::getValueType() const
{
    return Type::getVoidType();
}

std::string SSACheckExpectedTypeSelectorValueInstruction::getMnemonic() const
{
    return "checkExpectedTypeSelectorValue";
}

void SSACheckExpectedTypeSelectorValueInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(aggregate);
}

void SSACheckExpectedTypeSelectorValueInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(aggregate);
}

const SSAValuePtr &SSACheckExpectedTypeSelectorValueInstruction::getAggregate() const
{
    return aggregate;
}

void SSACheckExpectedTypeSelectorValueInstruction::setAggregate(const SSAValuePtr &newAggregate)
{
    aggregate = newAggregate;
}

uint64_t SSACheckExpectedTypeSelectorValueInstruction::getTypeSelectorIndex() const
{
    return typeSelectorIndex;
}

void SSACheckExpectedTypeSelectorValueInstruction::setTypeSelectorIndex(uint64_t newTypeSelectorIndex)
{
    typeSelectorIndex = newTypeSelectorIndex;
}

const TypePtr &SSACheckExpectedTypeSelectorValueInstruction::getTypeSelectorReferenceType() const
{
    return typeSelectorReferenceType;
}

void SSACheckExpectedTypeSelectorValueInstruction::setTypeSelectorReferenceType(const TypePtr &newTypeSelectorReferenceType)
{
    typeSelectorReferenceType = newTypeSelectorReferenceType;
}

uint64_t SSACheckExpectedTypeSelectorValueInstruction::getExpectedTypeSelector() const
{
    return expectedTypeSelector;
}

void SSACheckExpectedTypeSelectorValueInstruction::setExpectedTypeSelector(uint64_t newExpectedTypeSelector)
{
    expectedTypeSelector = newExpectedTypeSelector;
}

} // End of namespace Environment
} // End of namespace Sysmel