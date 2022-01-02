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
class SYSMEL_COMPILER_LIB_EXPORT SSASendMessageInstruction : public SubtypeOf<SSAInstruction, SSASendMessageInstruction>
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

    const FunctionalTypePtr &getCalledFunctionalType() const;
    void setCalledFunctionalType(const FunctionalTypePtr &newCalledFunctionalType);
    
    void setVirtualTableUsage(bool newUseVirtualTable, uint32_t newVirtualTableSlotIndex, uint32_t newVirtualTableEntrySlotIndex);
    bool isUsingVirtualTable() const;
    uint32_t getVirtualTableSlotIndex() const;
    uint32_t getVirtualTableEntrySlotIndex() const;
    
protected:
    TypePtr valueType;

    FunctionalTypePtr calledFunctionalType;
    SSAValuePtr selector;
    SSAValuePtr receiver;
    SSAValuePtrList arguments;

    bool useVirtualTable = false;
    uint32_t virtualTableSlotIndex = 0;
    uint32_t virtualTableEntrySlotIndex = 0;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_SEND_MESSAGE_INSTRUCTION_HPP