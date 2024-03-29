#ifndef SYSMEL_ENVIRONMENT_SSA_RETURN_FROM_REGION_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_RETURN_FROM_REGION_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSAReturnFromRegionInstruction : public SubtypeOf<SSAInstruction, SSAReturnFromRegionInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAReturnFromRegionInstruction";

    virtual bool isSSAReturnFromRegionInstruction() const override;
    virtual bool isTerminatorInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    const SSAValuePtr &getValue() const;
    void setValue(const SSAValuePtr &newValue);

protected:
    SSAValuePtr value;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_RETURN_FROM_REGION_INSTRUCTION_HPP