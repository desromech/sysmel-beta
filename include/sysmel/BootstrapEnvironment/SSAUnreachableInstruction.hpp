#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_UNREACHABLE_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_UNREACHABLE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAUnreachableInstruction : public SubtypeOf<SSAInstruction, SSAUnreachableInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAUnreachableInstruction";

    virtual bool isSSAUnreachableInstruction() const override;
    virtual bool isTerminatorInstruction() const override;

    virtual std::string getMnemonic() const override;

    virtual TypePtr getValueType() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_UNREACHABLE_INSTRUCTION_HPP