#ifndef SYSMEL_ENVIRONMENT_SSA_LOAD_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_LOAD_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSALoadInstruction : public SubtypeOf<SSAInstruction, SSALoadInstruction>
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

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_LOAD_INSTRUCTION_HPP