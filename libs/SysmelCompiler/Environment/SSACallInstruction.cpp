#include "Environment/SSACallInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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
    if(function)
        function->addUse(selfFromThis());
}

const SSAValuePtrList &SSACallInstruction::getArguments() const
{
    return arguments;
}

void SSACallInstruction::setArguments(const SSAValuePtrList &newArguments)
{
    arguments = newArguments;
    for(auto &arg : arguments)  
        arg->addUse(selfFromThis());
}

bool SSACallInstruction::isImplicitCleanUp() const
{
    return implicitCleanUp;
}

void SSACallInstruction::setImplicitCleanUp(bool newImplicitCleanUp)
{
    implicitCleanUp = newImplicitCleanUp;
}


} // End of namespace Environment
} // End of namespace Sysmel