#include "Environment/SSAMakeVectorInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAMakeVectorInstruction> SSAMakeVectorInstructionTypeRegistration;

bool SSAMakeVectorInstruction::isSSAMakeVectorInstruction() const
{
    return true;
}

AnyValuePtr SSAMakeVectorInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitMakeVectorInstruction(selfFromThis());
}

std::string SSAMakeVectorInstruction::getMnemonic() const
{
    return "makeVector";
}

void SSAMakeVectorInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    for (auto & element : elements)
        aBlock(element);
}

void SSAMakeVectorInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    for (auto & element : elements)
        aBlock(element);
}

TypePtr SSAMakeVectorInstruction::getValueType() const
{
    return valueType;
}

void SSAMakeVectorInstruction::setValueType(const TypePtr &newValueType)
{
    valueType = newValueType;
}

const SSAValuePtrList &SSAMakeVectorInstruction::getElements() const
{
    return elements;
}

void SSAMakeVectorInstruction::setElements(const SSAValuePtrList &newElements)
{
    elements = newElements;
    for(auto &el: elements)
        el->addUse(selfFromThis());
}

} // End of namespace Environment
} // End of namespace Sysmel