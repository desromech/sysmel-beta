#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/SSAFunction.hpp"


namespace Sysmel
{
namespace Environment
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
        ssaCompiledFunction->setExternalLanguageMode(externalLanguageMode);
        ssaCompiledFunction->setVisibility(visibility);
        ssaCompiledFunction->setDllLinkageMode(dllLinkageMode);
        ssaCompiledFunction->setCompileTime(isCompileTimeMethod());
    }

    return ssaCompiledFunction;
}

} // End of namespace Environment
} // End of namespace Sysmel