#include "sysmel/BootstrapEnvironment/SSABuilder.hpp"
#include "sysmel/BootstrapEnvironment/SSABasicBlock.hpp"
#include "sysmel/BootstrapEnvironment/SSACodeRegion.hpp"
#include "sysmel/BootstrapEnvironment/SSAFunction.hpp"

#include "sysmel/BootstrapEnvironment/SSAInstruction.hpp"

#include "sysmel/BootstrapEnvironment/SSAConstantLiteralValue.hpp"
#include "sysmel/BootstrapEnvironment/SSABreakInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSACallInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAContinueInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSADoWithCleanupInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSADoWhileInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAIfInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSALoadInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSALocalVariableInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAMakeAggregateInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAMakeClosureInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAReturnFromFunctionInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAReturnFromRegionInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSASendMessageInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAStoreInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAUnreachableInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAWhileInstruction.hpp"

#include "sysmel/BootstrapEnvironment/SSABitcastInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSADowncastInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAUpcastInstruction.hpp"

#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/PointerLikeType.hpp"

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

SSACodeRegionPtr SSABuilder::makeCodeRegionWithSignature(const TypePtrList &argumentTypes, const TypePtr &resultType)
{
    auto region = makeCodeRegion();
    region->setSignature(argumentTypes, resultType);
    return region;
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
    assert(currentPosition || !isLastTerminator());
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

SSABreakInstructionPtr SSABuilder::breakInstruction()
{
    auto instruction = basicMakeObject<SSABreakInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    addInstruction(instruction);
    return instruction;   
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

SSAContinueInstructionPtr SSABuilder::continueInstruction()
{
    auto instruction = basicMakeObject<SSAContinueInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    addInstruction(instruction);
    return instruction;   
}

SSADoWithCleanupInstructionPtr SSABuilder::doWithCleanUp(const SSACodeRegionPtr &bodyRegion, const SSACodeRegionPtr &cleanUpRegion)
{
    auto instruction = basicMakeObject<SSADoWithCleanupInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setBodyRegion(bodyRegion);
    instruction->setCleanUpRegion(cleanUpRegion);
    addInstruction(instruction);
    return instruction;

}

SSADoWhileInstructionPtr SSABuilder::doWhileContinueWith(const SSACodeRegionPtr &bodyRegion, const SSACodeRegionPtr &conditionRegion, const SSACodeRegionPtr &continueRegion)
{
    auto instruction = basicMakeObject<SSADoWhileInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setConditionRegion(conditionRegion);
    instruction->setBodyRegion(bodyRegion);
    instruction->setContinueRegion(continueRegion);
    addInstruction(instruction);
    return instruction;
}

SSAIfInstructionPtr SSABuilder::ifTrueIfFalse(const TypePtr &resultType, const SSAValuePtr &condition, const SSACodeRegionPtr &trueRegion, const SSACodeRegionPtr &falseRegion)
{
    auto instruction = basicMakeObject<SSAIfInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setValueType(resultType);
    instruction->setCondition(condition);
    instruction->setTrueRegion(trueRegion);
    instruction->setFalseRegion(falseRegion);
    addInstruction(instruction);
    return instruction;
}

SSALoadInstructionPtr SSABuilder::load(const SSAValuePtr &reference)
{
    auto referenceType = reference->getValueType();
    assert(referenceType->isPointerLikeType());
    auto resultType = referenceType.staticAs<PointerLikeType> ()->getBaseType();

    auto instruction = basicMakeObject<SSALoadInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setValueType(resultType);
    instruction->setReference(reference);
    addInstruction(instruction);
    return instruction;
}

SSALocalVariableInstructionPtr SSABuilder::localVariable(const TypePtr &referenceType, const TypePtr &valueType)
{
    assert(referenceType->isPointerLikeType());
    assert(referenceType.staticAs<PointerLikeType> ()->getBaseType()->isSubtypeOf(valueType));

    auto instruction = basicMakeObject<SSALocalVariableInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setVariableReferenceType(referenceType);
    instruction->setVariableValueType(valueType);
    addInstruction(instruction);
    return instruction;
}

SSAMakeClosureInstructionPtr SSABuilder::makeClosure(const SSAValuePtr &closureImplementation, const SSAValuePtrList &capturedValues)
{
    auto instruction = basicMakeObject<SSAMakeClosureInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setClosureImplementation(closureImplementation);
    instruction->setCapturedValues(capturedValues);
    addInstruction(instruction);
    return instruction;
}

SSAMakeAggregateInstructionPtr SSABuilder::makeAggregate(const TypePtr &aggregateType, const SSAValuePtrList &elements)
{
    assert(aggregateType->asUndecoratedType()->isAggregateType());

    auto instruction = basicMakeObject<SSAMakeAggregateInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setValueType(aggregateType);
    instruction->setElements(elements);
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

SSAStoreInstructionPtr SSABuilder::storeValueIn(const SSAValuePtr &value, const SSAValuePtr &reference)
{
    auto instruction = basicMakeObject<SSAStoreInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setValue(value);
    instruction->setReference(reference);
    addInstruction(instruction);
    return instruction;   
}

SSAUnreachableInstructionPtr SSABuilder::unreachableInstruction()
{
    auto instruction = basicMakeObject<SSAUnreachableInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    addInstruction(instruction);
    return instruction;   
}

SSAWhileInstructionPtr SSABuilder::whileDoContinueWith(const SSACodeRegionPtr &conditionRegion, const SSACodeRegionPtr &bodyRegion, const SSACodeRegionPtr &continueRegion)
{
    auto instruction = basicMakeObject<SSAWhileInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setConditionRegion(conditionRegion);
    instruction->setBodyRegion(bodyRegion);
    instruction->setContinueRegion(continueRegion);
    addInstruction(instruction);
    return instruction;
}

SSABitcastInstructionPtr SSABuilder::bitcast(const TypePtr &targetType, const SSAValuePtr &value)
{
    auto instruction = basicMakeObject<SSABitcastInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setTargetType(targetType);
    instruction->setValue(value);
    addInstruction(instruction);
    return instruction;
}

SSAUpcastInstructionPtr SSABuilder::upcast(const TypePtr &targetType, const SSAValuePtr &value)
{
    auto instruction = basicMakeObject<SSAUpcastInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setTargetType(targetType);
    instruction->setValue(value);
    addInstruction(instruction);
    return instruction;
}

SSADowncastInstructionPtr SSABuilder::downcast(const TypePtr &targetType, const SSAValuePtr &value)
{
    auto instruction = basicMakeObject<SSADowncastInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setTargetType(targetType);
    instruction->setValue(value);
    addInstruction(instruction);
    return instruction;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius