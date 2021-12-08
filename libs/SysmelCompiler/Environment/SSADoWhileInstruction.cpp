#include "Environment/SSADoWhileInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSADoWhileInstruction> SSADoWhileInstructionTypeRegistration;

bool SSADoWhileInstruction::isSSADoWhileInstruction() const
{
    return true;
}

AnyValuePtr SSADoWhileInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitDoWhileInstruction(selfFromThis());
}


TypePtr SSADoWhileInstruction::getValueType() const
{
    return Type::getVoidType();
}

std::string SSADoWhileInstruction::getMnemonic() const
{
    return "doWhile";
}

void SSADoWhileInstruction::regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const
{
    aBlock(bodyRegion);
    aBlock(conditionRegion);
    aBlock(continueRegion);
}

const SSACodeRegionPtr &SSADoWhileInstruction::getConditionRegion() const
{
    return conditionRegion;
}

void SSADoWhileInstruction::setConditionRegion(const SSACodeRegionPtr &newConditionRegion)
{
    conditionRegion = newConditionRegion;
}

const SSACodeRegionPtr &SSADoWhileInstruction::getBodyRegion() const
{
    return bodyRegion;
}

void SSADoWhileInstruction::setBodyRegion(const SSACodeRegionPtr &newBodyRegion)
{
    bodyRegion = newBodyRegion;
}

const SSACodeRegionPtr &SSADoWhileInstruction::getContinueRegion() const
{
    return continueRegion;
}

void SSADoWhileInstruction::setContinueRegion(const SSACodeRegionPtr &newContinueRegion)
{
    continueRegion = newContinueRegion;
}

} // End of namespace Environment
} // End of namespace Sysmel
