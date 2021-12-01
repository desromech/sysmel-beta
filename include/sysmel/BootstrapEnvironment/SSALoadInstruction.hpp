#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_LOAD_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_LOAD_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSALoadInstruction : public SubtypeOf<SSAInstruction, SSALoadInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSALoadInstruction";

    virtual bool isSSALoadInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;
    void setValueType(const TypePtr &newValueType);

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    const SSAValuePtr &getReference() const;
    void setReference(const SSAValuePtr &newReference);

protected:
    TypePtr valueType;
    SSAValuePtr reference;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_LOAD_INSTRUCTION_HPP