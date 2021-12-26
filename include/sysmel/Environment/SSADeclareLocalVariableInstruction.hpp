#ifndef SYSMEL_ENVIRONMENT_SSA_DECLARE_LOCAL_VARIABLE_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_DECLARE_LOCAL_VARIABLE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I declare the existence of a specific local variable. I am used to facilitate the generation of debug information.
 */
class SSADeclareLocalVariableInstruction : public SubtypeOf<SSAInstruction, SSADeclareLocalVariableInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSADeclareLocalVariableInstruction";

    virtual bool isSSADeclareLocalVariableInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    const SSAValuePtr &getValue() const;
    void setValue(const SSAValuePtr &newValue);

    const VariablePtr &getVariable() const;
    void setVariable(const VariablePtr &newVariable);

protected:
    SSAValuePtr value;
    VariablePtr variable;

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_DECLARE_LOCAL_VARIABLE_INSTRUCTION_HPP