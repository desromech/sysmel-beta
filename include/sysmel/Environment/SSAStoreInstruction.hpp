#ifndef SYSMEL_ENVIRONMENT_SSA_STORE_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_STORE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSAStoreInstruction : public SubtypeOf<SSAInstruction, SSAStoreInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAStoreInstruction";

    virtual bool isSSAStoreInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual TypePtr getValueType() const override;

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    const SSAValuePtr &getValue() const;
    void setValue(const SSAValuePtr &newValue);

    const SSAValuePtr &getReference() const;
    void setReference(const SSAValuePtr &newReference);

protected:
    SSAValuePtr value;
    SSAValuePtr reference;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_STORE_INSTRUCTION_HPP