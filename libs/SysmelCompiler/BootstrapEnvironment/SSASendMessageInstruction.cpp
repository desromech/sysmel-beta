#include "sysmel/BootstrapEnvironment/SSASendMessageInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
}

const SSAValuePtr &SSASendMessageInstruction::getReceiver() const
{
    return receiver;
}

void SSASendMessageInstruction::setReceiver(const SSAValuePtr &newReceiver)
{
    receiver = newReceiver;
}

const SSAValuePtrList &SSASendMessageInstruction::getAguments() const
{
    return arguments;
}

void SSASendMessageInstruction::setArguments(const SSAValuePtrList &newArguments)
{
    arguments = newArguments;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius