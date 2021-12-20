#include "Environment/SSALocalFinalization.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSALocalFinalization> SSALocalFinalizationTypeRegistration;

bool SSALocalFinalization::isSSALocalFinalization() const
{
    return true;
}

AnyValuePtr SSALocalFinalization::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitLocalFinalization(selfFromThis());
}

TypePtr SSALocalFinalization::getValueType() const
{
    return Type::getVoidType();
}

std::string SSALocalFinalization::getMnemonic() const
{
    return "localFinalization";
}

void SSALocalFinalization::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(localVariable);
}

void SSALocalFinalization::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(localVariable);
}

void SSALocalFinalization::regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const
{
    aBlock(finalizationRegion);
}

const SSAValuePtr &SSALocalFinalization::getLocalVariable() const
{
    return localVariable;
}

void SSALocalFinalization::setLocalVariable(const SSAValuePtr &newLocalVariable)
{
    localVariable = newLocalVariable;
}

const SSACodeRegionPtr &SSALocalFinalization::getFinalizationRegion() const
{
    return finalizationRegion;
}

void SSALocalFinalization::setFinalizationRegion(const SSACodeRegionPtr &newFinalizationRegion)
{
    finalizationRegion = newFinalizationRegion;
}

} // End of namespace Environment
} // End of namespace Sysmel