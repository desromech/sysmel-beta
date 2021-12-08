#include "Environment/SSACastInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSACastInstruction> SSACastInstructionTypeRegistration;

bool SSACastInstruction::isSSACastInstruction() const
{
    return true;
}

TypePtr SSACastInstruction::getValueType() const
{
    return targetType;
}

void SSACastInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(value);
}

void SSACastInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(value);
}

void SSACastInstruction::setTargetType(const TypePtr &newTargetType)
{
    targetType = newTargetType;
}

const TypePtr &SSACastInstruction::getTargetType() const
{
    return targetType;
}

const SSAValuePtr &SSACastInstruction::getValue() const
{
    return value;
}

void SSACastInstruction::setValue(const SSAValuePtr &newValue)
{
    value = newValue;
}

} // End of namespace Environment
} // End of namespace Sysmel