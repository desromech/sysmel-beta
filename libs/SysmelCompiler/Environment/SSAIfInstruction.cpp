#include "Environment/SSAIfInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

const TypePtr &SSAIfInstruction::getDeclaredValueType() const
{
    return declaredValueType;
}

void SSAIfInstruction::setDeclaredValueType(const TypePtr &newValueType)
{
    declaredValueType = valueType = newValueType;
    if(valueType->isReturnedByReference())
        valueType = valueType->tempRef();
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
    if(condition)
        condition->addUse(selfFromThis());
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

} // End of namespace Environment
} // End of namespace Sysmel