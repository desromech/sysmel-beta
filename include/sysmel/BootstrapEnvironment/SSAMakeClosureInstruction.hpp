#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_MAKE_CLOSURE_INSTRUCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_MAKE_CLOSURE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAMakeClosureInstruction : public SubtypeOf<SSAInstruction, SSAMakeClosureInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAMakeClosureInstruction";

    virtual bool isSSAMakeClosureInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual std::string getMnemonic() const override;

    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    TypePtr getValueType() const override;

    const SSAValuePtr &getClosureImplementation() const;
    void setClosureImplementation(const SSAValuePtr &newClosureImplementation);

    const SSAValuePtrList &getCapturedValues() const;
    void setCapturedValues(const SSAValuePtrList &newCapturedValues);

private:
    TypePtr valueType;
    SSAValuePtr closureImplementation;
    SSAValuePtrList capturedValues;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_MAKE_CLOSURE_INSTRUCTION_HPP