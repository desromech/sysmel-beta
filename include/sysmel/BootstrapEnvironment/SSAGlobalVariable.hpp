#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_GLOBAL_VARIABLE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_GLOBAL_VARIABLE_HPP
#pragma once

#include "SSAProgramEntity.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAGlobalVariable : public SubtypeOf<SSAProgramEntity, SSAGlobalVariable>
{
public:
    static constexpr char const __typeName__[] = "SSAGlobalVariable";

    virtual bool isSSAGlobalVariable() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_GLOBAL_VARIABLE_HPP