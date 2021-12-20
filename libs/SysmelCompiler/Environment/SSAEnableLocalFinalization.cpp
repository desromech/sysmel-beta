#include "Environment/SSAEnableLocalFinalization.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAEnableLocalFinalization> SSAEnableLocalFinalizationTypeRegistration;

bool SSAEnableLocalFinalization::isSSAEnableLocalFinalization() const
{
    return true;
}

AnyValuePtr SSAEnableLocalFinalization::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitEnableLocalFinalization(selfFromThis());
}

TypePtr SSAEnableLocalFinalization::getValueType() const
{
    return Type::getVoidType();
}

std::string SSAEnableLocalFinalization::getMnemonic() const
{
    return "enableLocalFinalization";
}

void SSAEnableLocalFinalization::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(localVariable);
}

void SSAEnableLocalFinalization::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(localVariable);
}

const SSAValuePtr &SSAEnableLocalFinalization::getLocalVariable() const
{
    return localVariable;
}

void SSAEnableLocalFinalization::setLocalVariable(const SSAValuePtr &newLocalVariable)
{
    localVariable = newLocalVariable;
    if(localVariable)
        localVariable->addUse(selfFromThis());
}

} // End of namespace Environment
} // End of namespace Sysmel