#ifndef SYSMEL_ENVIRONMENT_SSA_VECTOR_SWIZZLE_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_VECTOR_SWIZZLE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am an instruction that selects and reorders vector elements
 */
class SYSMEL_COMPILER_LIB_EXPORT SSAVectorSwizzleInstruction : public SubtypeOf<SSAInstruction, SSAVectorSwizzleInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAVectorSwizzleInstruction";

    virtual bool isSSAVectorSwizzleInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual std::string getMnemonic() const override;

    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    TypePtr getValueType() const override;
    void setValueType(const TypePtr &newValueType);

    const SSAValuePtr &getVector() const;
    void setVector(const SSAValuePtr &newVector);

    const std::vector<uint32_t> &getSelectedElements() const;
    void setSelectedElements(const std::vector<uint32_t> &newSelectedElements);

private:
    TypePtr valueType;
    SSAValuePtr vector;
    std::vector<uint32_t> selectedElements;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_VECTOR_SWIZZLE_INSTRUCTION_HPP