#ifndef SYSMEL_ENVIRONMENT_GLOBAL_VARIABLE_HPP
#define SYSMEL_ENVIRONMENT_GLOBAL_VARIABLE_HPP
#pragma once

#include "Variable.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(GlobalVariable);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(DeferredCompileTimeCodeFragment);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAGlobalVariable);

/**
 * I am a variable program entity.
 */
class SYSMEL_COMPILER_LIB_EXPORT GlobalVariable : public SubtypeOf<Variable, GlobalVariable>
{
public:
    static constexpr char const __typeName__[] = "GlobalVariable";

    virtual bool isGlobalVariable() const override;
    virtual AnyValuePtr findStoreBindingInCompileTime(const CompileTimeCleanUpScopePtr &compileTimeCleanUpScope) override;

    virtual void ensureSemanticAnalysis() override;
    virtual SSAValuePtr asSSAValueRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition) override;

    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;

    DeferredCompileTimeCodeFragmentPtr initialValueCodeFragment;

protected:
    void ensureEvaluationOfInitialValue();

    bool hasEvaluatedInitialValueCodeFragment = false;

    AnyValuePtr currentValueOrValueBox;
    AnyValuePtr referenceValue;
    SSAGlobalVariablePtr ssaGlobalVariable;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_GLOBAL_VARIABLE_HPP