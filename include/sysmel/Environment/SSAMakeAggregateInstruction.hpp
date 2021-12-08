#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_MAKE_AGGREGATE_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_MAKE_AGGREGATE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAMakeAggregateInstruction : public SubtypeOf<SSAInstruction, SSAMakeAggregateInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAMakeAggregateInstruction";

    virtual bool isSSAMakeAggregateInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    TypePtr getValueType() const override;
    void setValueType(const TypePtr &type);

    const SSAValuePtrList &getElements() const;
    void setElements(const SSAValuePtrList &newElements);

private:
    TypePtr valueType;
    SSAValuePtrList elements;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_MAKE_AGGREGATE_INSTRUCTION_HPP