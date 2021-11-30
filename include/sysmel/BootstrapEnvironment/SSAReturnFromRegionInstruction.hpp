#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_RETURN_FROM_REGION_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_RETURN_FROM_REGION_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAReturnFromRegionInstruction : public SubtypeOf<SSAInstruction, SSAReturnFromRegionInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAReturnFromRegionInstruction";

    virtual bool isSSAReturnFromRegionInstruction() const override;
    virtual bool isTerminatorInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    const SSAValuePtr &getValue() const;
    void setValue(const SSAValuePtr &newValue);

protected:
    SSAValuePtr value;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_RETURN_FROM_REGION_INSTRUCTION_HPP