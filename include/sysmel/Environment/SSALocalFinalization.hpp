#ifndef SYSMEL_ENVIRONMENT_SSA_LOCAL_FINALIZATION_HPP
#define SYSMEL_ENVIRONMENT_SSA_LOCAL_FINALIZATION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSALocalFinalization : public SubtypeOf<SSAInstruction, SSALocalFinalization>
{
public:
    static constexpr char const __typeName__[] = "SSALocalFinalization";

    virtual bool isSSALocalFinalization() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;
    void setValueType(const TypePtr &newValueType);

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    virtual void regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const;

    const SSAValuePtr &getLocalVariable() const;
    void setLocalVariable(const SSAValuePtr &newLocalVariable);

    const SSACodeRegionPtr &getFinalizationRegion() const;
    void setFinalizationRegion(const SSACodeRegionPtr &newFinalizationRegion);

protected:
    SSAValuePtr localVariable;
    SSACodeRegionPtr finalizationRegion;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_LOCAL_FINALIZATION_HPP