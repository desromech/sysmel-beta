#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILATION_ERROR_VALUE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILATION_ERROR_VALUE_HPP
#pragma once

#include "AnyValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompilationErrorValue);


/**
 * I am literal number value.
 */
class CompilationErrorValue : public SubtypeOf<AnyValue, CompilationErrorValue>
{
public:
    static constexpr char const __typeName__[] = "CompilationErrorValue";
    static constexpr bool __isDynamicCompileTimeType__ = false;

    static CompilationErrorValuePtr uniqueInstance();

    virtual bool isCompilationErrorValue() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_COMPILATION_ERROR_VALUE_HPP