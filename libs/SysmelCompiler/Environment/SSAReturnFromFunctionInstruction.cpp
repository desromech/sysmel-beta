#include "Environment/SSAReturnFromFunctionInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

TypePtr SSAReturnFromFunctionInstruction::getValueType() const
{
    return Type::getVoidType();
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

} // End of namespace Environment
} // End of namespace Sysmel