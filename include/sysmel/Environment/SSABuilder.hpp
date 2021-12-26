#ifndef SYSMEL_ENVIRONMENT_SSA_BUILDER_HPP
#define SYSMEL_ENVIRONMENT_SSA_BUILDER_HPP
#pragma once

#include "SSAValue.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FieldVariable);

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSABuilder : public SubtypeOf<CompilerObject, SSABuilder>
{
public:
    static constexpr char const __typeName__[] = "SSABuilder";

    void setSourcePosition(const ASTSourcePositionPtr &sourcePosition);
    const ASTSourcePositionPtr &getSourcePosition() const;

    void setCodeRegion(const SSACodeRegionPtr &region);
    const SSACodeRegionPtr &getCodeRegion() const;

    void setBasicBlock(const SSABasicBlockPtr &basicBlock);
    const SSABasicBlockPtr &getBasicBlock() const;

    void setPosition(const SSAInstructionPtr &position);
    const SSAInstructionPtr &getPosition() const;

    bool isLastTerminator() const;

    SSACodeRegionPtr makeCodeRegion();
    SSACodeRegionPtr makeCodeRegionWithSignature(const TypePtrList &argumentTypes, const TypePtr &resultType);

    SSABasicBlockPtr makeBasicBlock();
    SSABasicBlockPtr makeBasicBlockHere();

    void addInstruction(const SSAInstructionPtr &instruction);

    SSAValuePtr normalizeValue(const SSAValuePtr &value);

    SSAConstantLiteralValuePtr literalWithType(const AnyValuePtr &value, const TypePtr &type);
    SSAConstantLiteralValuePtr literal(const AnyValuePtr &value);

    SSABreakInstructionPtr breakInstruction();
    SSACallInstructionPtr call(const TypePtr &resultType, const SSAValuePtr &function, const SSAValuePtrList &arguments);
    SSAContinueInstructionPtr continueInstruction();
    SSADeclareLocalVariableInstructionPtr declareLocalVariable(const SSAValuePtr &value, const VariablePtr &variable);
    SSADoWithCleanupInstructionPtr doWithCleanUp(const SSACodeRegionPtr &bodyRegion, const SSACodeRegionPtr &cleanUpRegion);
    SSADoWhileInstructionPtr doWhileContinueWith(const SSACodeRegionPtr &bodyRegion, const SSACodeRegionPtr &conditionRegion, const SSACodeRegionPtr &continueRegion);
    SSAGetAggregateFieldReferenceInstructionPtr getAggregateFieldReference(const TypePtr &valueType, const SSAValuePtr &aggregate, const FieldVariablePtr &field);
    SSAGetAggregateSlotReferenceInstructionPtr getAggregateSlotReference(const TypePtr &valueType, const SSAValuePtr &aggregate, const SSAValuePtr &slotIndex);
    SSAIfInstructionPtr ifTrueIfFalse(const TypePtr &resultType, const SSAValuePtr &condition, const SSACodeRegionPtr &trueRegion, const SSACodeRegionPtr &falseRegion);
    SSALoadInstructionPtr load(const SSAValuePtr &reference);
    SSALocalVariableInstructionPtr localVariable(const TypePtr &referenceType, const TypePtr &valueType);
    SSAMakeClosureInstructionPtr makeClosure(const SSAValuePtr &closureImplementation, const SSAValuePtrList &capturedValues);
    SSAReturnFromFunctionInstructionPtr returnFromFunction(const SSAValuePtr &value);
    SSAReturnFromRegionInstructionPtr returnFromRegion(const SSAValuePtr &value);
    SSASendMessageInstructionPtr sendMessage(const TypePtr &resultType, const SSAValuePtr &selector, const SSAValuePtr &receiver, const SSAValuePtrList &arguments);
    SSAStoreInstructionPtr storeValueIn(const SSAValuePtr &value, const SSAValuePtr &reference);
    SSAUnreachableInstructionPtr unreachableInstruction();
    SSAWhileInstructionPtr whileDoContinueWith(const SSACodeRegionPtr &conditionRegion, const SSACodeRegionPtr &bodyRegion, const SSACodeRegionPtr &continueRegion);

    SSABitcastInstructionPtr bitcast(const TypePtr &targetType, const SSAValuePtr &value);
    SSAUpcastInstructionPtr upcast(const TypePtr &targetType, const SSAValuePtr &value);
    SSADowncastInstructionPtr downcast(const TypePtr &targetType, const SSAValuePtr &value);

    SSAEnableLocalFinalizationPtr enableLocalFinalization(const SSAValuePtr &localVariable);
    SSALocalFinalizationPtr localFinalization(const SSAValuePtr &localVariable, const SSACodeRegionPtr &finalizationCodeRegion);

protected:
    SSACodeRegionPtr currentRegion;
    SSABasicBlockPtr currentBasicBlock;
    SSAInstructionPtr currentPosition;
    ASTSourcePositionPtr currentSourcePosition;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_BUILDER_HPP