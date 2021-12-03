#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_VARIABLE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_VARIABLE_HPP
#pragma once

#include "Variable.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a variable program entity.
 */
class FunctionVariable : public SubtypeOf<Variable, FunctionVariable>
{
public:
    static constexpr char const __typeName__[] = "FunctionVariable";

    virtual bool isFunctionVariable() const override;
    virtual AnyValuePtr findStoreBindingInCompileTime(const CompileTimeCleanUpScopePtr &compileTimeCleanUpScope) override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_VARIABLE_HPP