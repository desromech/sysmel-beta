#include "sysmel/BootstrapEnvironment/SSAIfInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAIfInstruction> SSAIfInstructionTypeRegistration;

bool SSAIfInstruction::isSSAIfInstruction() const
{
    return true;
}

AnyValuePtr SSAIfInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitIfInstruction(selfFromThis());
}

TypePtr SSAIfInstruction::getValueType() const
{
    return valueType;
}

void SSAIfInstruction::setValueType(const TypePtr &newValueType)
{
    valueType = newValueType;
}

std::string SSAIfInstruction::getMnemonic() const
{
    return "if";
}

void SSAIfInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(condition);
}

void SSAIfInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(condition);
}

void SSAIfInstruction::regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const
{
    aBlock(trueRegion);
    aBlock(falseRegion);
}

const SSAValuePtr &SSAIfInstruction::getCondition() const
{
    return condition;
}

void SSAIfInstruction::setCondition(const SSAValuePtr &newCondition)
{
    condition = newCondition;
}

const SSACodeRegionPtr &SSAIfInstruction::getTrueRegion() const
{
    return trueRegion;
}

void SSAIfInstruction::setTrueRegion(const SSACodeRegionPtr &newTrueRegion)
{
    trueRegion = newTrueRegion;
}

const SSACodeRegionPtr &SSAIfInstruction::getFalseRegion() const
{
    return falseRegion;
}

void SSAIfInstruction::setFalseRegion(const SSACodeRegionPtr &newFalseRegion)
{
    falseRegion = newFalseRegion;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius