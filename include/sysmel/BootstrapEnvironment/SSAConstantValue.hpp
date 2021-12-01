#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CONSTANT_VALUE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CONSTANT_VALUE_HPP
#pragma once

#include "SSAValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAConstantValue : public SubtypeOf<SSAValue, SSAConstantValue>
{
public:
    static constexpr char const __typeName__[] = "SSAConstantValue";

    virtual bool isSSAConstantValue() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CONSTANT_VALUE_HPP