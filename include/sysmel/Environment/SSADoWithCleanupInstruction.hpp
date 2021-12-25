#ifndef SYSMEL_ENVIRONMENT_SSA_DO_WHILE_CLEANUP_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_DO_WHILE_CLEANUP_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSADoWithCleanupInstruction : public SubtypeOf<SSAInstruction, SSADoWithCleanupInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSADoWithCleanupInstruction";

    virtual bool isSSADoWithCleanupInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;

    virtual std::string getMnemonic() const override;
    
    virtual void regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const override;

    const SSACodeRegionPtr &getBodyRegion() const;
    void setBodyRegion(const SSACodeRegionPtr &newBodyRegion);

    const SSACodeRegionPtr &getCleanUpRegion() const;
    void setCleanUpRegion(const SSACodeRegionPtr &newCleanUpRegion);

protected:
    SSACodeRegionPtr bodyRegion;
    SSACodeRegionPtr cleanUpRegion;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_DO_WHILE_CLEANUP_STATEMENT_INSTRUCTION_HPP