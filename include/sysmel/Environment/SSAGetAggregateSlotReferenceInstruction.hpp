#ifndef SYSMEL_ENVIRONMENT_SSA_GET_AGGREGATE_SLOT_REFERENCE_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_GET_AGGREGATE_SLOT_REFERENCE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FieldVariable)

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAGetAggregateSlotReferenceInstruction : public SubtypeOf<SSAInstruction, SSAGetAggregateSlotReferenceInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAGetAggregateSlotReferenceInstruction";

    virtual bool isSSAGetAggregateSlotReferenceInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    TypePtr getValueType() const override;
    void setValueType(const TypePtr &type);

    const SSAValuePtr &getAggregate() const;
    void setAggregate(const SSAValuePtr &newAggregate);

    const SSAValuePtr &getSlotIndex() const;
    void setSlotIndex(const SSAValuePtr &newSlotIndex);

private:
    TypePtr valueType;
    SSAValuePtr aggregate;
    SSAValuePtr slotIndex;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_GET_AGGREGATE_SLOT_REFERENCE_INSTRUCTION_HPP