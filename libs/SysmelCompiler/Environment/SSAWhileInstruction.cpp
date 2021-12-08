#include "Environment/SSAWhileInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAWhileInstruction> SSAWhileInstructionTypeRegistration;

bool SSAWhileInstruction::isSSAWhileInstruction() const
{
    return true;
}

AnyValuePtr SSAWhileInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitWhileInstruction(selfFromThis());
}

TypePtr SSAWhileInstruction::getValueType() const
{
    return Type::getVoidType();
}

std::string SSAWhileInstruction::getMnemonic() const
{
    return "while";
}

void SSAWhileInstruction::regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const
{
    aBlock(conditionRegion);
    aBlock(bodyRegion);
    aBlock(continueRegion);
}

const SSACodeRegionPtr &SSAWhileInstruction::getConditionRegion() const
{
    return conditionRegion;
}

void SSAWhileInstruction::setConditionRegion(const SSACodeRegionPtr &newConditionRegion)
{
    conditionRegion = newConditionRegion;
}

const SSACodeRegionPtr &SSAWhileInstruction::getBodyRegion() const
{
    return bodyRegion;
}

void SSAWhileInstruction::setBodyRegion(const SSACodeRegionPtr &newBodyRegion)
{
    bodyRegion = newBodyRegion;
}

const SSACodeRegionPtr &SSAWhileInstruction::getContinueRegion() const
{
    return continueRegion;
}

void SSAWhileInstruction::setContinueRegion(const SSACodeRegionPtr &newContinueRegion)
{
    continueRegion = newContinueRegion;
}

} // End of namespace Environment
} // End of namespace Sysmel