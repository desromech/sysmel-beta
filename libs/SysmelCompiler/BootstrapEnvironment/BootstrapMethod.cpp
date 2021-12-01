#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/SSAFunction.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<BootstrapMethodBase> bootstrapMethodBaseTypeRegistration;

bool BootstrapMethodBase::isBootstrapMethod() const
{
    return true;
}

bool BootstrapMethodBase::isCompileTimeEvaluableMethod() const
{
    return true;
}

SSAValuePtr BootstrapMethodBase::asSSAValueRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition)
{
    (void)requiredSourcePosition;
    if(!ssaCompiledFunction)
    {
        ssaCompiledFunction = basicMakeObject<SSAFunction> ();
        ssaCompiledFunction->initializeWithNameAndType(getName(), getFunctionalType());
        ssaCompiledFunction->setIntrinsicName(getIntrinsicName());
        ssaCompiledFunction->setSourceProgramEntity(selfFromThis());
    }

    return ssaCompiledFunction;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius