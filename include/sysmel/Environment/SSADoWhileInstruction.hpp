#ifndef SYSMEL_ENVIRONMENT_SSA_DO_WHILE_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_DO_WHILE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSADoWhileInstruction : public SubtypeOf<SSAInstruction, SSADoWhileInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSADoWhileInstruction";

    virtual bool isSSADoWhileInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;
    virtual std::string getMnemonic() const override;
    
    virtual void regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const override;

    const SSACodeRegionPtr &getConditionRegion() const;
    void setConditionRegion(const SSACodeRegionPtr &newConditionRegion);

    const SSACodeRegionPtr &getBodyRegion() const;
    void setBodyRegion(const SSACodeRegionPtr &newBodyRegion);

    const SSACodeRegionPtr &getContinueRegion() const;
    void setContinueRegion(const SSACodeRegionPtr &newContinueRegion);

protected:
    SSACodeRegionPtr bodyRegion;
    SSACodeRegionPtr conditionRegion;
    SSACodeRegionPtr continueRegion;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_DO_WHILE_INSTRUCTION_HPP