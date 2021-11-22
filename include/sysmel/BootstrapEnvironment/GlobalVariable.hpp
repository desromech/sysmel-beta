#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_GLOBAL_VARIABLE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_GLOBAL_VARIABLE_HPP
#pragma once

#include "Variable.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(GlobalVariable);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(DeferredCompileTimeCodeFragment);

/**
 * I am a variable program entity.
 */
class GlobalVariable : public SubtypeOf<Variable, GlobalVariable>
{
public:
    static constexpr char const __typeName__[] = "GlobalVariable";

    virtual bool isGlobalVariable() const override;
    virtual AnyValuePtr findStoreBindingInCompileTime(const CompileTimeCleanUpScopePtr &compileTimeCleanUpScope) override;

    virtual void ensureSemanticAnalysis() override;

    DeferredCompileTimeCodeFragmentPtr initialValueCodeFragment;

protected:
    void ensureEvaluationOfInitialValue();

    bool hasEvaluatedInitialValueCodeFragment = false;

    AnyValuePtr currentValueOrValueBox;
    AnyValuePtr referenceValue;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_GLOBAL_VARIABLE_HPP