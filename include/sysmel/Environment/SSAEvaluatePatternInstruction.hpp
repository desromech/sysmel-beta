#ifndef SYSMEL_ENVIRONMENT_SSA_EVALUATE_PATTERN_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_EVALUATE_PATTERN_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSAEvaluatePatternInstruction : public SubtypeOf<SSAInstruction, SSAEvaluatePatternInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAEvaluatePatternInstruction";

    virtual bool isSSAEvaluatePatternInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;
    void setValueType(const TypePtr &newValueType);

    virtual std::string getMnemonic() const override;
    virtual void regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const override;

    const SSACodeRegionPtr &getPatternRegion() const;
    void setPatternRegion(const SSACodeRegionPtr &newPatternRegion);

    const SSACodeRegionPtr &getSuccessRegion() const;
    void setSuccessRegion(const SSACodeRegionPtr &newSuccessRegion);

    const SSACodeRegionPtr &getFailureRegion() const;
    void setFailureRegion(const SSACodeRegionPtr &newFailureRegion);

protected:
    TypePtr valueType;
    SSACodeRegionPtr patternRegion;
    SSACodeRegionPtr successRegion;
    SSACodeRegionPtr failureRegion;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_EVALUATE_PATTERN_INSTRUCTION_HPP