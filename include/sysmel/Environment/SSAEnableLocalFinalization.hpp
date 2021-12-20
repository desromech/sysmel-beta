#ifndef SYSMEL_ENVIRONMENT_SSA_ENABLE_LOCAL_FINALIZATION_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_ENABLE_LOCAL_FINALIZATION_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAEnableLocalFinalization : public SubtypeOf<SSAInstruction, SSAEnableLocalFinalization>
{
public:
    static constexpr char const __typeName__[] = "SSAEnableLocalFinalization";

    virtual bool isSSAEnableLocalFinalization() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    const SSAValuePtr &getLocalVariable() const;
    void setLocalVariable(const SSAValuePtr &newLocalVariable);
    
protected:
    SSAValuePtr localVariable;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_ENABLE_LOCAL_FINALIZATION_INSTRUCTION_HPP