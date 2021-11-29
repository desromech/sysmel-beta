#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_INSTRUCTION_HPP
#pragma once

#include "SSACodeRegionLocalValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAInstruction : public SubtypeOf<SSACodeRegionLocalValue, SSAInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAInstruction";

    virtual bool isSSAInstruction() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_INSTRUCTION_HPP