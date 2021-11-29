#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_GLOBAL_VALUE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_GLOBAL_VALUE_HPP
#pragma once

#include "SSAConstantValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAGlobalValue : public SubtypeOf<SSAConstantValue, SSAGlobalValue>
{
public:
    static constexpr char const __typeName__[] = "SSAGlobalValue";

    virtual bool isSSAGlobalValue() const override;
    virtual AnyValuePtr getName() const;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_GLOBAL_VALUE_HPP