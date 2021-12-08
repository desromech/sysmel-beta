#include "Environment/SSADoWithCleanupInstruction.hpp"
#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSADoWithCleanupInstruction> SSADoWithCleanupInstructionTypeRegistration;

bool SSADoWithCleanupInstruction::isSSADoWithCleanupInstruction() const
{
    return true;
}

AnyValuePtr SSADoWithCleanupInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitDoWithCleanupInstruction(selfFromThis());
}


TypePtr SSADoWithCleanupInstruction::getValueType() const
{
    return bodyRegion->getResultType();
}

std::string SSADoWithCleanupInstruction::getMnemonic() const
{
    return "doWithCleanUp";
}

void SSADoWithCleanupInstruction::regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const
{
    aBlock(bodyRegion);
    aBlock(cleanUpRegion);
}

const SSACodeRegionPtr &SSADoWithCleanupInstruction::getBodyRegion() const
{
    return bodyRegion;
}

void SSADoWithCleanupInstruction::setBodyRegion(const SSACodeRegionPtr &newBodyRegion)
{
    bodyRegion = newBodyRegion;
}

const SSACodeRegionPtr &SSADoWithCleanupInstruction::getCleanUpRegion() const
{
    return cleanUpRegion;
}

void SSADoWithCleanupInstruction::setCleanUpRegion(const SSACodeRegionPtr &newCleanUpRegion)
{
    cleanUpRegion = newCleanUpRegion;
}

} // End of namespace Environment
} // End of namespace Sysmel