#ifndef SYSMEL_ENVIRONMENT_SSA_CALL_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_CALL_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSACallInstruction : public SubtypeOf<SSAInstruction, SSACallInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSACallInstruction";

    virtual bool isSSACallInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    TypePtr getValueType() const override;
    void setValueType(const TypePtr &type);

    const SSAValuePtr &getFunction() const;
    void setFunction(const SSAValuePtr &newFunction);

    const SSAValuePtrList &getArguments() const;
    void setArguments(const SSAValuePtrList &newArguments);
    
    bool isImplicitCleanUp() const;
    void setImplicitCleanUp(bool newImplicitCleanUp);
    
protected:
    TypePtr valueType;

    SSAValuePtr function;
    SSAValuePtrList arguments;
    bool implicitCleanUp = false;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_INSTRUCTION_HPP