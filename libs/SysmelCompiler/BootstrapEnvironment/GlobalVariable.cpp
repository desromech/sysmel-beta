#include "sysmel/BootstrapEnvironment/GlobalVariable.hpp"
#include "sysmel/BootstrapEnvironment/DeferredCompileTimeCodeFragment.hpp"
#include "sysmel/BootstrapEnvironment/ValueBox.hpp"
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
        auto box = std::make_shared<ValueBox> ();
        box->type = valueType;
        box->value = evaluatedValue;
        currentValueOrValueBox = box;
    }
    else
    {
        currentValueOrValueBox = evaluatedValue;
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
    return currentValueOrValueBox;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius