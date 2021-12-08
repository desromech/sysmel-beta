#ifndef SYSMEL_ENVIRONMENT_SSA_SEND_MESSAGE_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_SEND_MESSAGE_INSTRUCTION_HPP
#pragma once

#include "SSAInstruction.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSASendMessageInstruction : public SubtypeOf<SSAInstruction, SSASendMessageInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSASendMessageInstruction";

    virtual bool isSSASendMessageInstruction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;

    virtual std::string getMnemonic() const override;
    
    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock) override;
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const override;

    TypePtr getValueType() const override;
    void setValueType(const TypePtr &type);

    const SSAValuePtr &getSelector() const;
    void setSelector(const SSAValuePtr &newValue);

    const SSAValuePtr &getReceiver() const;
    void setReceiver(const SSAValuePtr &newReceiver);

    const SSAValuePtrList &getArguments() const;
    void setArguments(const SSAValuePtrList &newArguments);
    
protected:
    TypePtr valueType;

    SSAValuePtr selector;
    SSAValuePtr receiver;
    SSAValuePtrList arguments;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_SEND_MESSAGE_INSTRUCTION_HPP