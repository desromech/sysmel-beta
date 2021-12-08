#ifndef SYSMEL_ENVIRONMENT_SSA_IF_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_IF_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAIfInstruction : public SubtypeOf<SSAInstruction, SSAIfInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAIfInstruction";

    virtual bool isSSAIfInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;
    void setValueType(const TypePtr &newValueType);

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    virtual void regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const;

    const SSAValuePtr &getCondition() const;
    void setCondition(const SSAValuePtr &newCondition);

    const SSACodeRegionPtr &getTrueRegion() const;
    void setTrueRegion(const SSACodeRegionPtr &newTrueRegion);

    const SSACodeRegionPtr &getFalseRegion() const;
    void setFalseRegion(const SSACodeRegionPtr &newFalseRegion);

protected:
    TypePtr valueType;
    SSAValuePtr condition;
    SSACodeRegionPtr trueRegion;
    SSACodeRegionPtr falseRegion;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_IF_INSTRUCTION_HPP