#include "Environment/GlobalVariable.hpp"
#include "Environment/DeferredCompileTimeCodeFragment.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"


namespace Sysmel
{
namespace Environment
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
        sysmelAssert(referenceType->isReferenceLikeType());
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

} // End of namespace Environment
} // End of namespace Sysmel