#include "sysmel/BootstrapEnvironment/SSADoWithCleanupInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSACodeRegion.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius