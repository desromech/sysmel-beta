#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CAST_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CAST_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSACastInstruction : public SubtypeOf<SSAInstruction, SSACastInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSACastInstruction";

    virtual bool isSSACastInstruction() const override;

    virtual TypePtr getValueType() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    void setTargetType(const TypePtr &newTargetType);
    const TypePtr &getTargetType() const;

    const SSAValuePtr &getValue() const;
    void setValue(const SSAValuePtr &newValue);

protected:
    TypePtr targetType;
    SSAValuePtr value;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CAST_INSTRUCTION_HPP