#ifndef SYSMEL_ENVIRONMENT_SSA_CAST_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_CAST_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSACastInstruction : public SubtypeOf<SSAInstruction, SSACastInstruction>
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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_CAST_INSTRUCTION_HPP