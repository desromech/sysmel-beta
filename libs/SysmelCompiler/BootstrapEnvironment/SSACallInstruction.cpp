#include "sysmel/BootstrapEnvironment/SSACallInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSACallInstruction> SSACallInstructionTypeRegistration;

bool SSACallInstruction::isSSACallInstruction() const
{
    return true;
}

AnyValuePtr SSACallInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitCallInstruction(selfFromThis());
}


std::string SSACallInstruction::getMnemonic() const
{
    return "call";
}

void SSACallInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(function);
    for(auto &arg : arguments)
        aBlock(arg);
}

void SSACallInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(function);
    for(auto &arg : arguments)
        aBlock(arg);
}

TypePtr SSACallInstruction::getValueType() const
{
    return valueType;
}

void SSACallInstruction::setValueType(const TypePtr &type)
{
    valueType = type;
}

const SSAValuePtr &SSACallInstruction::getFunction() const
{
    return function;
}

void SSACallInstruction::setFunction(const SSAValuePtr &newFunction)
{
    function = newFunction;
}

const SSAValuePtrList &SSACallInstruction::getAguments() const
{
    return arguments;
}

void SSACallInstruction::setArguments(const SSAValuePtrList &newArguments)
{
    arguments = newArguments;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius