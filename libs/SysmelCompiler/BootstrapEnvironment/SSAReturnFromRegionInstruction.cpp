#include "sysmel/BootstrapEnvironment/SSAReturnFromRegionInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAReturnFromRegionInstruction> SSAReturnFromRegionInstructionTypeRegistration;

bool SSAReturnFromRegionInstruction::isSSAReturnFromRegionInstruction() const
{
    return true;
}

bool SSAReturnFromRegionInstruction::isTerminatorInstruction() const
{
    return true;
}

AnyValuePtr SSAReturnFromRegionInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitReturnFromRegionInstruction(selfFromThis());
}

std::string SSAReturnFromRegionInstruction::getMnemonic() const
{
    return "returnFromRegion";
}

void SSAReturnFromRegionInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(value);
}

void SSAReturnFromRegionInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(value);
}

const SSAValuePtr &SSAReturnFromRegionInstruction::getValue() const
{
    return value;
}

void SSAReturnFromRegionInstruction::setValue(const SSAValuePtr &newValue)
{
    value = newValue;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius