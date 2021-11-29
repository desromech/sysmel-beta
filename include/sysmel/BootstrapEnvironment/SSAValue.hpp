#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_VALUE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_VALUE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAValue : public SubtypeOf<CompilerObject, SSAValue>
{
public:
    static constexpr char const __typeName__[] = "SSAValue";

    virtual bool isSSAValue() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_VALUE_HPP