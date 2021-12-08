#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_GET_AGGREGATE_FIELD_REFERENCE_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_GET_AGGREGATE_FIELD_REFERENCE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FieldVariable)

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAGetAggregateFieldReferenceInstruction : public SubtypeOf<SSAInstruction, SSAGetAggregateFieldReferenceInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAGetAggregateFieldReferenceInstruction";

    virtual bool isSSAGetAggregateFieldReferenceInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    TypePtr getValueType() const override;
    void setValueType(const TypePtr &type);

    const SSAValuePtr &getAggregate() const;
    void setAggregate(const SSAValuePtr &newAggregate);

    const FieldVariablePtr &getFieldVariable() const;
    void setFieldVariable(const FieldVariablePtr &newFieldVariable);
    
private:
    TypePtr valueType;
    SSAValuePtr aggregate;
    FieldVariablePtr fieldVariable;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_GET_AGGREGATE_FIELD_REFERENCE_INSTRUCTION_HPP