#include "sysmel/BootstrapEnvironment/SSABuilder.hpp"
#include "sysmel/BootstrapEnvironment/SSABasicBlock.hpp"
#include "sysmel/BootstrapEnvironment/SSACodeRegion.hpp"

#include "sysmel/BootstrapEnvironment/SSAInstruction.hpp"

#include "sysmel/BootstrapEnvironment/SSAConstantLiteralValue.hpp"
#include "sysmel/BootstrapEnvironment/SSACallInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAReturnFromFunctionInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAReturnFromRegionInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSASendMessageInstruction.hpp"

#include "sysmel/BootstrapEnvironment/Type.hpp"

#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSABuilder> SSABuilderTypeRegistration;

void SSABuilder::setSourcePosition(const ASTSourcePositionPtr &sourcePosition)
{
    currentSourcePosition = sourcePosition;
}

const ASTSourcePositionPtr &SSABuilder::getSourcePosition() const
{
    return currentSourcePosition;
}

void SSABuilder::setCodeRegion(const SSACodeRegionPtr &region)
{
    currentRegion = region;
    currentBasicBlock.reset();
    currentPosition.reset();
}

const SSACodeRegionPtr &SSABuilder::getCodeRegion() const
{
    return currentRegion;
}

void SSABuilder::setBasicBlock(const SSABasicBlockPtr &basicBlock)
{
    setCodeRegion(basicBlock ? basicBlock->getParentCodeRegion() : nullptr);
    currentBasicBlock = basicBlock;
}

const SSABasicBlockPtr &SSABuilder::getBasicBlock() const
{
    return currentBasicBlock;
}

void SSABuilder::setPosition(const SSAInstructionPtr &position)
{
    setBasicBlock(position ? position->getParentBasicBlock() : nullptr);
    currentPosition = position;
}

const SSAInstructionPtr &SSABuilder::getPosition() const
{
    return currentPosition;
}

SSACodeRegionPtr SSABuilder::makeCodeRegion()
{
    return basicMakeObject<SSACodeRegion> ();
}

SSABasicBlockPtr SSABuilder::makeBasicBlock()
{
    return basicMakeObject<SSABasicBlock> ();
}

SSABasicBlockPtr SSABuilder::makeBasicBlockHere()
{
    auto block = basicMakeObject<SSABasicBlock> ();
    currentRegion->addBasicBlock(block);
    setBasicBlock(block);
    return block;
}

bool SSABuilder::isLastTerminator() const
{
    if(!currentBasicBlock)
        return false;

    auto lastInstruction = currentBasicBlock->getLastInstruction();
    return lastInstruction && lastInstruction->isTerminatorInstruction();
}

void SSABuilder::addInstruction(const SSAInstructionPtr &instruction)
{
    currentBasicBlock->addInstructionBefore(instruction, currentPosition);
}

SSAValuePtr SSABuilder::normalizeValue(const SSAValuePtr &value)
{
    if(value->getValueType()->isVoidType() && !value->isSSAConstantLiteralValue())
        return literal(getVoidConstant());
    return value;
}

SSAConstantLiteralValuePtr SSABuilder::literalWithType(const AnyValuePtr &value, const TypePtr &type)
{
    return SSAConstantLiteralValue::makeWith(value, type, currentSourcePosition);
}

SSAConstantLiteralValuePtr SSABuilder::literal(const AnyValuePtr &value)
{
    return literalWithType(value, validAnyValue(value)->getType());
}

SSACallInstructionPtr SSABuilder::call(const TypePtr &resultType, const SSAValuePtr &function, const SSAValuePtrList &arguments)
{
    auto instruction = basicMakeObject<SSACallInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setValueType(resultType);
    instruction->setFunction(function);
    instruction->setArguments(arguments);
    addInstruction(instruction);
    return instruction;   
}

SSAReturnFromFunctionInstructionPtr SSABuilder::returnFromFunction(const SSAValuePtr &value)
{
    auto instruction = basicMakeObject<SSAReturnFromFunctionInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setValue(normalizeValue(value));
    addInstruction(instruction);
    return instruction;
}

SSAReturnFromRegionInstructionPtr SSABuilder::returnFromRegion(const SSAValuePtr &value)
{
    auto instruction = basicMakeObject<SSAReturnFromRegionInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setValue(normalizeValue(value));
    addInstruction(instruction);
    return instruction;
}

SSASendMessageInstructionPtr SSABuilder::sendMessage(const TypePtr &resultType, const SSAValuePtr &selector, const SSAValuePtr &receiver, const SSAValuePtrList &arguments)
{
    auto instruction = basicMakeObject<SSASendMessageInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setValueType(resultType);
    instruction->setSelector(selector);
    instruction->setReceiver(receiver);
    instruction->setArguments(arguments);
    addInstruction(instruction);
    return instruction;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius