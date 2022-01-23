#include "Environment/SSAEvaluatePatternInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAEvaluatePatternInstruction> SSAEvaluatePatternInstructionTypeRegistration;

bool SSAEvaluatePatternInstruction::isSSAEvaluatePatternInstruction() const
{
    return true;
}

AnyValuePtr SSAEvaluatePatternInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitEvaluatePatternInstruction(selfFromThis());
}

TypePtr SSAEvaluatePatternInstruction::getValueType() const
{
    return valueType;
}

void SSAEvaluatePatternInstruction::setValueType(const TypePtr &newValueType)
{
    valueType = newValueType;
}

std::string SSAEvaluatePatternInstruction::getMnemonic() const
{
    return "evaluatePattern";
}

void SSAEvaluatePatternInstruction::regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const
{
    aBlock(patternRegion);
    aBlock(successRegion);
    aBlock(failureRegion);
}

const SSACodeRegionPtr &SSAEvaluatePatternInstruction::getPatternRegion() const
{
    return patternRegion;
}

void SSAEvaluatePatternInstruction::setPatternRegion(const SSACodeRegionPtr &newPatternRegion)
{
    patternRegion = newPatternRegion;
}

const SSACodeRegionPtr &SSAEvaluatePatternInstruction::getSuccessRegion() const
{
    return successRegion;
}

void SSAEvaluatePatternInstruction::setSuccessRegion(const SSACodeRegionPtr &newSuccessRegion)
{
    successRegion = newSuccessRegion;
}

const SSACodeRegionPtr &SSAEvaluatePatternInstruction::getFailureRegion() const
{
    return failureRegion;
}

void SSAEvaluatePatternInstruction::setFailureRegion(const SSACodeRegionPtr &newFailureRegion)
{
    failureRegion = newFailureRegion;
}

} // End of namespace Environment
} // End of namespace Sysmel