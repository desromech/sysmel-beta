#include "sysmel/BootstrapEnvironment/GlobalVariable.hpp"
#include "sysmel/BootstrapEnvironment/DeferredCompileTimeCodeFragment.hpp"
#include "sysmel/BootstrapEnvironment/PointerLikeType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<GlobalVariable> localVariableTypeRegistration;

void GlobalVariable::ensureSemanticAnalysis()
{
    SuperType::ensureSemanticAnalysis();
    ensureEvaluationOfInitialValue();
}

void GlobalVariable::ensureEvaluationOfInitialValue()
{
    if(hasEvaluatedInitialValueCodeFragment || !initialValueCodeFragment)
        return;

    auto evaluatedValue = initialValueCodeFragment->analyzeAndEvaluate();
    if(isMutable_)
    {
        assert(referenceType->isReferenceLikeType());
        currentValueOrValueBox = evaluatedValue->asMutableStoreValue();
        referenceValue = staticObjectCast<PointerLikeType> (referenceType)->makeWithValue(currentValueOrValueBox);
    }
    else
    {
        referenceValue = currentValueOrValueBox = evaluatedValue;
    }

    hasEvaluatedInitialValueCodeFragment = true;
}

bool GlobalVariable::isGlobalVariable() const
{
    return true;
}

AnyValuePtr GlobalVariable::findStoreBindingInCompileTime(const CompileTimeCleanUpScopePtr &compileTimeCleanUpScope)
{
    (void)compileTimeCleanUpScope;
    ensureEvaluationOfInitialValue();
    return referenceValue;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius