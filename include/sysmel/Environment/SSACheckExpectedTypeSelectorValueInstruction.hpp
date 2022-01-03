#ifndef SYSMEL_ENVIRONMENT_SSA_CHECK_EXPECTED_TYPE_SELECTOR_VALUE_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_CHECK_EXPECTED_TYPE_SELECTOR_VALUE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I check that a given type selector on a variant has an expected value, otherwise I hit a trap.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSACheckExpectedTypeSelectorValueInstruction : public SubtypeOf<SSAInstruction, SSACheckExpectedTypeSelectorValueInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSACheckExpectedTypeSelectorValueInstruction";

    virtual bool isSSACheckExpectedTypeSelectorValueInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    const SSAValuePtr &getAggregate() const;
    void setAggregate(const SSAValuePtr &aggregate);

    uint64_t getTypeSelectorIndex() const;
    void setTypeSelectorIndex(uint64_t newTypeSelectorIndex);

    const TypePtr &getTypeSelectorReferenceType() const;
    void setTypeSelectorReferenceType(const TypePtr &newTypeSelectorReferenceType);

    uint64_t getExpectedTypeSelector() const;
    void setExpectedTypeSelector(uint64_t newExpectedTypeSelector);

protected:
    SSAValuePtr aggregate;
    uint64_t typeSelectorIndex;
    TypePtr typeSelectorReferenceType;
    uint64_t expectedTypeSelector;

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_CHECK_EXPECTED_TYPE_SELECTOR_VALUE_INSTRUCTION_HPP