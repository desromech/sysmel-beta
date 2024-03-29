#include "Environment/SSASendMessageInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSASendMessageInstruction> SSASendMessageInstructionTypeRegistration;

bool SSASendMessageInstruction::isSSASendMessageInstruction() const
{
    return true;
}

AnyValuePtr SSASendMessageInstruction::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitSendMessageInstruction(selfFromThis());
}

std::string SSASendMessageInstruction::getMnemonic() const
{
    return "sendMessage";
}

void SSASendMessageInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    aBlock(selector);
    aBlock(receiver);
    for(auto &arg : arguments)
        aBlock(arg);
}

void SSASendMessageInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    aBlock(selector);
    aBlock(receiver);
    for(auto &arg : arguments)
        aBlock(arg);
}

TypePtr SSASendMessageInstruction::getValueType() const
{
    return valueType;
}

void SSASendMessageInstruction::setValueType(const TypePtr &type)
{
    valueType = type;
}

const SSAValuePtr &SSASendMessageInstruction::getSelector() const
{
    return selector;
}

void SSASendMessageInstruction::setSelector(const SSAValuePtr &newSelector)
{
    selector = newSelector;
    if(selector)
        selector->addUse(selfFromThis());
}

const SSAValuePtr &SSASendMessageInstruction::getReceiver() const
{
    return receiver;
}

void SSASendMessageInstruction::setReceiver(const SSAValuePtr &newReceiver)
{
    receiver = newReceiver;
    if(receiver)
        receiver->addUse(selfFromThis());
}

const SSAValuePtrList &SSASendMessageInstruction::getArguments() const
{
    return arguments;
}

void SSASendMessageInstruction::setArguments(const SSAValuePtrList &newArguments)
{
    arguments = newArguments;
    for(auto &arg : arguments)
        arg->addUse(selfFromThis());
}

const FunctionalTypePtr &SSASendMessageInstruction::getCalledFunctionalType() const
{
    return calledFunctionalType;
}

void SSASendMessageInstruction::setCalledFunctionalType(const FunctionalTypePtr &newCalledFunctionalType)
{
    calledFunctionalType = newCalledFunctionalType;
}

void SSASendMessageInstruction::setVirtualTableUsage(bool newUseVirtualTable, uint32_t newVirtualTableSlotIndex, uint32_t newVirtualTableEntrySlotIndex)
{
    useVirtualTable = newUseVirtualTable;
    virtualTableSlotIndex = newVirtualTableSlotIndex;
    virtualTableEntrySlotIndex = newVirtualTableEntrySlotIndex;
}

bool SSASendMessageInstruction::isUsingVirtualTable() const
{
    return useVirtualTable;
}

uint32_t SSASendMessageInstruction::getVirtualTableSlotIndex() const
{
    return virtualTableSlotIndex;
}

uint32_t SSASendMessageInstruction::getVirtualTableEntrySlotIndex() const
{
    return virtualTableEntrySlotIndex;
}

} // End of namespace Environment
} // End of namespace Sysmel