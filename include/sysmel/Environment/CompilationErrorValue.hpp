#ifndef SYSMEL_ENVIRONMENT_COMPILATION_ERROR_VALUE_HPP
#define SYSMEL_ENVIRONMENT_COMPILATION_ERROR_VALUE_HPP
#pragma once

#include "AnyValue.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(CompilationErrorValue);


/**
 * I am literal number value.
 */
class SYSMEL_COMPILER_LIB_EXPORT CompilationErrorValue : public SubtypeOf<AnyValue, CompilationErrorValue>
{
public:
    static constexpr char const __typeName__[] = "CompilationErrorValue";
    static constexpr bool __isDynamicCompileTimeType__ = false;

    static CompilationErrorValuePtr uniqueInstance();

    virtual bool isCompilationErrorValue() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_COMPILATION_ERROR_VALUE_HPP