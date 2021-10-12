#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_TYPE_HPP
#pragma once

#include "FunctionalType.hpp"
#include "SpecificMethod.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionTypeValue);

/**
 * I am an instance of a function type object.
 */
class FunctionType : public SubtypeOf<FunctionalType, FunctionType>
{
public:
    virtual bool isFunctionType() const override;

    static FunctionTypePtr makeForMethodSignature(const MethodSignature &signature);
};

/**
 * I am an instance of a function type object.
 */
class FunctionTypeValue : public SubtypeOf<FunctionalTypeValue, FunctionTypeValue>
{
public:
    virtual bool isFunctionTypeValue() const override;

    virtual TypePtr getType() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_TYPE_HPP