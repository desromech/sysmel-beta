#include "Environment/SSADeclareLocalVariableInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSADeclareLocalVariableInstruction> SSADeclareLocalVariableInstructionTypeRegistration;

bool SSADeclareLocalVariableInstruction::isSSADeclareLocalVariableInstruction() const
{
    return true;
}

AnyValuePtr SSADeclareLocalVariableInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitDeclareLocalVariableInstruction(selfFromThis());
}

TypePtr SSADeclareLocalVariableInstruction::getValueType() const
{
    return Type::getVoidType();
}

std::string SSADeclareLocalVariableInstruction::getMnemonic() const
{
    return "declareLocalVariable";
}

void SSADeclareLocalVariableInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(value);
}

void SSADeclareLocalVariableInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(value);
}

const SSAValuePtr &SSADeclareLocalVariableInstruction::getValue() const
{
    return value;
}

void SSADeclareLocalVariableInstruction::setValue(const SSAValuePtr &newValue)
{
    value = newValue;
}

const VariablePtr &SSADeclareLocalVariableInstruction::getVariable() const
{
    return variable;
}

void SSADeclareLocalVariableInstruction::setVariable(const VariablePtr &newVariable)
{
    variable = newVariable;
}

} // End of namespace Environment
} // End of namespace Sysmel