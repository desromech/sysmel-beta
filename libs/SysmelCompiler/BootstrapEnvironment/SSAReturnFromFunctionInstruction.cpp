#include "sysmel/BootstrapEnvironment/SSAReturnFromFunctionInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAReturnFromFunctionInstruction> SSAReturnFromFunctionInstructionTypeRegistration;

bool SSAReturnFromFunctionInstruction::isSSAReturnFromFunctionInstruction() const
{
    return true;
}

bool SSAReturnFromFunctionInstruction::isTerminatorInstruction() const
{
    return true;
}

AnyValuePtr SSAReturnFromFunctionInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitReturnFromFunctionInstruction(selfFromThis());
}

std::string SSAReturnFromFunctionInstruction::getMnemonic() const
{
    return "returnFromFunction";
}

void SSAReturnFromFunctionInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(value);
}

void SSAReturnFromFunctionInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(value);
}

const SSAValuePtr &SSAReturnFromFunctionInstruction::getValue() const
{
    return value;
}

void SSAReturnFromFunctionInstruction::setValue(const SSAValuePtr &newValue)
{
    value = newValue;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius