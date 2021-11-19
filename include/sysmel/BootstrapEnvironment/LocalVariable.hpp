#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LOCAL_VARIABLE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LOCAL_VARIABLE_HPP
#pragma once

#include "Variable.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

/**
 * I am a variable program entity.
 */
class LocalVariable : public SubtypeOf<Variable, LocalVariable>
{
public:
    static constexpr char const __typeName__[] = "LocalVariable";

    virtual bool isLocalVariable() const override;
    virtual AnyValuePtr findStoreBindingInCompileTime(const CompileTimeCleanUpScopePtr &compileTimeCleanUpScope) override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LOCAL_VARIABLE_HPP