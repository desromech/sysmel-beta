#include "Environment/SSABuilder.hpp"
#include "Environment/SSABasicBlock.hpp"
#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSAFunction.hpp"

#include "Environment/SSAInstruction.hpp"

#include "Environment/SSAConstantLiteralValue.hpp"
#include "Environment/SSABreakInstruction.hpp"
#include "Environment/SSACallInstruction.hpp"
#include "Environment/SSAContinueInstruction.hpp"
#include "Environment/SSADeclareLocalVariableInstruction.hpp"
#include "Environment/SSADoWithCleanupInstruction.hpp"
#include "Environment/SSADoWhileInstruction.hpp"
#include "Environment/SSAGetAggregateFieldReferenceInstruction.hpp"
#include "Environment/SSAGetAggregateSlotReferenceInstruction.hpp"
#include "Environment/SSAIfInstruction.hpp"
#include "Environment/SSALoadInstruction.hpp"
#include "Environment/SSALocalVariableInstruction.hpp"
#include "Environment/SSAMakeClosureInstruction.hpp"
#include "Environment/SSAMakeVectorInstruction.hpp"
#include "Environment/SSAReturnFromFunctionInstruction.hpp"
#include "Environment/SSAReturnFromRegionInstruction.hpp"
#include "Environment/SSASendMessageInstruction.hpp"
#include "Environment/SSAStoreInstruction.hpp"
#include "Environment/SSAUnreachableInstruction.hpp"
#include "Environment/SSAVectorSwizzleInstruction.hpp"
#include "Environment/SSAWhileInstruction.hpp"

#include "Environment/SSABitcastInstruction.hpp"
#include "Environment/SSADowncastInstruction.hpp"
#include "Environment/SSAUpcastInstruction.hpp"

#include "Environment/SSAEnableLocalFinalization.hpp"
#include "Environment/SSALocalFinalization.hpp"

#include "Environment/SSACheckExpectedTypeSelectorValueInstruction.hpp"

#include "Environment/SSAEvaluatePatternInstruction.hpp"
#include "Environment/SSAFailPatternInstruction.hpp"
#include "Environment/SSATrapInstruction.hpp"

#include "Environment/Type.hpp"
#include "Environment/PointerLikeType.hpp"

#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

const LexicalScopePtr &SSABuilder::getLexicalScope() const
{
    return currentLexicalScope;
}

void SSABuilder::setLexicalScope(const LexicalScopePtr &newLexicalScope)
{
    currentLexicalScope = newLexicalScope;
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
    sysmelAssert(currentPosition || !isLastTerminator());
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
    instruction->setLexicalScope(currentLexicalScope);
    addInstruction(instruction);
    return instruction;   
}

SSACallInstructionPtr SSABuilder::call(const TypePtr &resultType, const SSAValuePtr &function, const SSAValuePtrList &arguments)
{
    auto instruction = basicMakeObject<SSACallInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
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
    instruction->setLexicalScope(currentLexicalScope);
    addInstruction(instruction);
    return instruction;   
}

SSADeclareLocalVariableInstructionPtr SSABuilder::declareLocalVariable(const SSAValuePtr &value, const VariablePtr &variable)
{
    auto instruction = basicMakeObject<SSADeclareLocalVariableInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setValue(value);
    instruction->setVariable(variable);
    addInstruction(instruction);
    return instruction;
}

SSADoWithCleanupInstructionPtr SSABuilder::doWithCleanUp(const SSACodeRegionPtr &bodyRegion, const SSACodeRegionPtr &cleanUpRegion)
{
    auto instruction = basicMakeObject<SSADoWithCleanupInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setBodyRegion(bodyRegion);
    instruction->setCleanUpRegion(cleanUpRegion);
    addInstruction(instruction);
    return instruction;

}

SSADoWhileInstructionPtr SSABuilder::doWhileContinueWith(const SSACodeRegionPtr &bodyRegion, const SSACodeRegionPtr &conditionRegion, const SSACodeRegionPtr &continueRegion)
{
    auto instruction = basicMakeObject<SSADoWhileInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setConditionRegion(conditionRegion);
    instruction->setBodyRegion(bodyRegion);
    instruction->setContinueRegion(continueRegion);
    addInstruction(instruction);
    return instruction;
}

SSAGetAggregateFieldReferenceInstructionPtr SSABuilder::getAggregateFieldReference(const TypePtr &valueType, const SSAValuePtr &aggregate, const FieldVariablePtr &field)
{
    auto instruction = basicMakeObject<SSAGetAggregateFieldReferenceInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setValueType(valueType);
    instruction->setAggregate(aggregate);
    instruction->setFieldVariable(field);
    addInstruction(instruction);
    return instruction;
}

SSAGetAggregateSlotReferenceInstructionPtr SSABuilder::getAggregateSlotReference(const TypePtr &valueType, const SSAValuePtr &aggregate, const SSAValuePtr &slotIndex)
{
    auto instruction = basicMakeObject<SSAGetAggregateSlotReferenceInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setValueType(valueType);
    instruction->setAggregate(aggregate);
    instruction->setSlotIndex(slotIndex);
    addInstruction(instruction);
    return instruction;
}

SSAIfInstructionPtr SSABuilder::ifTrueIfFalse(const TypePtr &resultType, const SSAValuePtr &condition, const SSACodeRegionPtr &trueRegion, const SSACodeRegionPtr &falseRegion)
{
    auto instruction = basicMakeObject<SSAIfInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
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
    sysmelAssert(referenceType->isPointerLikeType());
    auto resultType = referenceType.staticAs<PointerLikeType> ()->getBaseType();

    auto instruction = basicMakeObject<SSALoadInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setValueType(resultType);
    instruction->setReference(reference);
    addInstruction(instruction);
    return instruction;
}

SSALocalVariableInstructionPtr SSABuilder::localVariable(const TypePtr &referenceType, const TypePtr &valueType)
{
    sysmelAssert(referenceType->isPointerLikeType());
    sysmelAssert(referenceType.staticAs<PointerLikeType> ()->getBaseType()->isSubtypeOf(valueType));

    auto instruction = basicMakeObject<SSALocalVariableInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setVariableReferenceType(referenceType);
    instruction->setVariableValueType(valueType);
    addInstruction(instruction);
    return instruction;
}

SSAMakeClosureInstructionPtr SSABuilder::makeClosure(const SSAValuePtr &closureImplementation, const SSAValuePtrList &capturedValues)
{
    auto instruction = basicMakeObject<SSAMakeClosureInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setClosureImplementation(closureImplementation);
    instruction->setCapturedValues(capturedValues);
    addInstruction(instruction);
    return instruction;
}

SSAMakeVectorInstructionPtr SSABuilder::makeVector(const TypePtr &vectorType, const SSAValuePtrList &elements)
{
    auto instruction = basicMakeObject<SSAMakeVectorInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setValueType(vectorType);
    instruction->setElements(elements);
    addInstruction(instruction);
    return instruction;
}

SSAReturnFromFunctionInstructionPtr SSABuilder::returnFromFunction(const SSAValuePtr &value)
{
    auto instruction = basicMakeObject<SSAReturnFromFunctionInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setValue(normalizeValue(value));
    addInstruction(instruction);
    return instruction;
}

SSAReturnFromRegionInstructionPtr SSABuilder::returnFromRegion(const SSAValuePtr &value)
{
    auto instruction = basicMakeObject<SSAReturnFromRegionInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setValue(normalizeValue(value));
    addInstruction(instruction);
    return instruction;
}

SSASendMessageInstructionPtr SSABuilder::sendMessage(const TypePtr &resultType, const FunctionalTypePtr &calledFunctionType, const SSAValuePtr &selector, const SSAValuePtr &receiver, const SSAValuePtrList &arguments, bool useVirtualTable, uint32_t virtualTableSlotIndex, uint32_t virtualTableEntrySlotIndex)
{
    auto instruction = basicMakeObject<SSASendMessageInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setValueType(resultType);
    instruction->setCalledFunctionalType(calledFunctionType);
    instruction->setSelector(selector);
    instruction->setReceiver(receiver);
    instruction->setArguments(arguments);
    instruction->setVirtualTableUsage(useVirtualTable, virtualTableSlotIndex, virtualTableEntrySlotIndex);
    addInstruction(instruction);
    return instruction;
}

SSAStoreInstructionPtr SSABuilder::storeValueIn(const SSAValuePtr &value, const SSAValuePtr &reference)
{
    auto instruction = basicMakeObject<SSAStoreInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setValue(value);
    instruction->setReference(reference);
    addInstruction(instruction);
    return instruction;   
}

SSAUnreachableInstructionPtr SSABuilder::unreachableInstruction()
{
    auto instruction = basicMakeObject<SSAUnreachableInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    addInstruction(instruction);
    return instruction;   
}

SSAVectorSwizzleInstructionPtr SSABuilder::vectorSwizzle(const TypePtr &resultType, const SSAValuePtr &vector, const std::vector<uint32_t> &selectedElements)
{
    auto instruction = basicMakeObject<SSAVectorSwizzleInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setValueType(resultType);
    instruction->setVector(vector);
    instruction->setSelectedElements(selectedElements);
    addInstruction(instruction);
    return instruction;
}

SSAWhileInstructionPtr SSABuilder::whileDoContinueWith(const SSACodeRegionPtr &conditionRegion, const SSACodeRegionPtr &bodyRegion, const SSACodeRegionPtr &continueRegion)
{
    auto instruction = basicMakeObject<SSAWhileInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
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
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setTargetType(targetType);
    instruction->setValue(value);
    addInstruction(instruction);
    return instruction;
}

SSAUpcastInstructionPtr SSABuilder::upcast(const TypePtr &targetType, const SSAValuePtr &value)
{
    auto instruction = basicMakeObject<SSAUpcastInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setTargetType(targetType);
    instruction->setValue(value);
    addInstruction(instruction);
    return instruction;
}

SSADowncastInstructionPtr SSABuilder::downcast(const TypePtr &targetType, const SSAValuePtr &value)
{
    auto instruction = basicMakeObject<SSADowncastInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setTargetType(targetType);
    instruction->setValue(value);
    addInstruction(instruction);
    return instruction;
}

SSAEnableLocalFinalizationPtr SSABuilder::enableLocalFinalization(const SSAValuePtr &localVariable)
{
    auto instruction = basicMakeObject<SSAEnableLocalFinalization> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setLocalVariable(localVariable);
    addInstruction(instruction);
    return instruction;
}

SSALocalFinalizationPtr SSABuilder::localFinalization(const SSAValuePtr &localVariable, const SSACodeRegionPtr &finalizationCodeRegion)
{
    auto instruction = basicMakeObject<SSALocalFinalization> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setLocalVariable(localVariable);
    instruction->setFinalizationRegion(finalizationCodeRegion);
    addInstruction(instruction);
    return instruction;
}

SSACheckExpectedTypeSelectorValueInstructionPtr SSABuilder::checkExpectedTypeSelectorValue(const SSAValuePtr &aggregate, uint64_t typeSelectorSlotIndex, const TypePtr &typeSelectorSlotReferenceType, uint64_t expectedTypeSelectorValue)
{
    auto instruction = basicMakeObject<SSACheckExpectedTypeSelectorValueInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setAggregate(aggregate);
    instruction->setTypeSelectorIndex(typeSelectorSlotIndex);
    instruction->setTypeSelectorReferenceType(typeSelectorSlotReferenceType);
    instruction->setExpectedTypeSelector(expectedTypeSelectorValue);
    addInstruction(instruction);
    return instruction;
}

SSAEvaluatePatternInstructionPtr SSABuilder::evaluatePattern(const TypePtr &resultType, SSACodeRegionPtr &patternRegion, const SSACodeRegionPtr &successRegion, const SSACodeRegionPtr &failureRegion)
{
    auto instruction = basicMakeObject<SSAEvaluatePatternInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setValueType(resultType);
    instruction->setPatternRegion(patternRegion);
    instruction->setSuccessRegion(successRegion);
    instruction->setFailureRegion(failureRegion);
    addInstruction(instruction);
    return instruction;
}

SSAFailPatternInstructionPtr SSABuilder::failPattern()
{
    auto instruction = basicMakeObject<SSAFailPatternInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    addInstruction(instruction);
    return instruction;
}

SSATrapInstructionPtr SSABuilder::trap(TrapReason reason)
{
    auto instruction = basicMakeObject<SSATrapInstruction> ();
    instruction->setSourcePosition(currentSourcePosition);
    instruction->setLexicalScope(currentLexicalScope);
    instruction->setReason(reason);
    addInstruction(instruction);
    return instruction;
}

} // End of namespace Environment
} // End of namespace Sysmel