#include "Environment/SSAVectorSwizzleInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAVectorSwizzleInstruction> SSAVectorSwizzleInstructionTypeRegistration;

bool SSAVectorSwizzleInstruction::isSSAVectorSwizzleInstruction() const
{
    return true;
}

AnyValuePtr SSAVectorSwizzleInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitVectorSwizzleInstruction(selfFromThis());
}

std::string SSAVectorSwizzleInstruction::getMnemonic() const
{
    return "vectorSwizzle";
}

void SSAVectorSwizzleInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(vector);
}

void SSAVectorSwizzleInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(vector);
}

TypePtr SSAVectorSwizzleInstruction::getValueType() const
{
    return valueType;
}

void SSAVectorSwizzleInstruction::setValueType(const TypePtr &newValueType)
{
    valueType = newValueType;
}

const SSAValuePtr &SSAVectorSwizzleInstruction::getVector() const
{
    return vector;
}

void SSAVectorSwizzleInstruction::setVector(const SSAValuePtr &newVector)
{
    vector = newVector;
    vector->addUse(selfFromThis());
}

const std::vector<uint32_t> &SSAVectorSwizzleInstruction::getSelectedElements() const
{
    return selectedElements;
}

void SSAVectorSwizzleInstruction::setSelectedElements(const std::vector<uint32_t> &newSelectedElements)
{
    selectedElements = newSelectedElements;
}

} // End of namespace Environment
} // End of namespace Sysmel