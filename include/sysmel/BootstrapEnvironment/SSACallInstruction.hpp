#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CALL_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CALL_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSACallInstruction : public SubtypeOf<SSAInstruction, SSACallInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSACallInstruction";

    virtual bool isSSACallInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_INSTRUCTION_HPP