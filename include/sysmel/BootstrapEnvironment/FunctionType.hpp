#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_TYPE_HPP
#pragma once

#include "FunctionalType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionType);

/**
 * I am the base interface for compile time object that is passed through the interpreter.
 */
class FunctionType : public SubtypeOf<FunctionalType, FunctionType>
{
public:
    static constexpr char const __typeName__[] = "FunctionType";
    static constexpr char const __sysmelTypeName__[] = "_FunctionType";

    virtual bool isFunctionType() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_TYPE_HPP