#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_BREAK_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_BREAK_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSABreakInstruction : public SubtypeOf<SSAInstruction, SSABreakInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSABreakInstruction";

    virtual bool isSSABreakInstruction() const override;
    virtual bool isTerminatorInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;

    virtual std::string getMnemonic() const override;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_BREAK_INSTRUCTION_HPP