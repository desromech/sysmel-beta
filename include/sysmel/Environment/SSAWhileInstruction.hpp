#ifndef SYSMEL_ENVIRONMENT_SSA_WHILE_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_WHILE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAWhileInstruction : public SubtypeOf<SSAInstruction, SSAWhileInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAWhileInstruction";

    virtual bool isSSAWhileInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;
    virtual std::string getMnemonic() const override;
    
    virtual void regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const;

    const SSACodeRegionPtr &getConditionRegion() const;
    void setConditionRegion(const SSACodeRegionPtr &newConditionRegion);

    const SSACodeRegionPtr &getBodyRegion() const;
    void setBodyRegion(const SSACodeRegionPtr &newBodyRegion);

    const SSACodeRegionPtr &getContinueRegion() const;
    void setContinueRegion(const SSACodeRegionPtr &newContinueRegion);

protected:
    SSACodeRegionPtr conditionRegion;
    SSACodeRegionPtr bodyRegion;
    SSACodeRegionPtr continueRegion;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_WHILE_INSTRUCTION_HPP