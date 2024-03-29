#include "Environment/SSAReturnFromRegionInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

TypePtr SSAReturnFromRegionInstruction::getValueType() const
{
    return Type::getVoidType();
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
    if(value)
        value->addUse(selfFromThis());
}

} // End of namespace Environment
} // End of namespace Sysmel