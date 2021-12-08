#include "Environment/SSAMakeClosureInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAMakeClosureInstruction> SSAMakeClosureInstructionTypeRegistration;

bool SSAMakeClosureInstruction::isSSAMakeClosureInstruction() const
{
    return true;
}

AnyValuePtr SSAMakeClosureInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitMakeClosureInstruction(selfFromThis());
}

std::string SSAMakeClosureInstruction::getMnemonic() const
{
    return "makeClosure";
}

void SSAMakeClosureInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(closureImplementation);
    for (auto & capture : capturedValues)
        aBlock(capture);
}

void SSAMakeClosureInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(closureImplementation);
    for (const auto & capture : capturedValues)
        aBlock(capture);
}

TypePtr SSAMakeClosureInstruction::getValueType() const
{
    return closureImplementation->getValueType();
}

const SSAValuePtr &SSAMakeClosureInstruction::getClosureImplementation() const
{
    return closureImplementation;
}

void SSAMakeClosureInstruction::setClosureImplementation(const SSAValuePtr &newClosureImplementation)
{
    closureImplementation = newClosureImplementation;
}

const SSAValuePtrList &SSAMakeClosureInstruction::getCapturedValues() const
{
    return capturedValues;
}

void SSAMakeClosureInstruction::setCapturedValues(const SSAValuePtrList &newCapturedValues)
{
    capturedValues = newCapturedValues;
}

} // End of namespace Environment
} // End of namespace Sysmel