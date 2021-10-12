#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTIONAL_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTIONAL_TYPE_HPP
#pragma once

#include "AnyValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionalType);

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class FunctionalType : public SubtypeOf<AnyValue, FunctionalType>
{
public:
    static constexpr char const __typeName__[] = "FunctionalType";
    static constexpr char const __sysmelTypeName__[] = "_FunctionalType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isFunctionalType() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTIONAL_TYPE_HPP