#ifndef SYSMEL_ENVIRONMENT_FUNCTION_VARIABLE_HPP
#define SYSMEL_ENVIRONMENT_FUNCTION_VARIABLE_HPP
#pragma once

#include "Variable.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_FUNCTION_VARIABLE_HPP