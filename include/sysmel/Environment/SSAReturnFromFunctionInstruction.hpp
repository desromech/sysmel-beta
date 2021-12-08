#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_RETURN_FROM_FUNCTION_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_RETURN_FROM_FUNCTION_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAReturnFromFunctionInstruction : public SubtypeOf<SSAInstruction, SSAReturnFromFunctionInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAReturnFromFunctionInstruction";

    virtual bool isSSAReturnFromFunctionInstruction() const override;
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

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_RETURN_FROM_FUNCTION_INSTRUCTION_HPP