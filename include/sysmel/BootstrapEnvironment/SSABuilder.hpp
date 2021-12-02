#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_BUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_BUILDER_HPP
#pragma once

#include "SSAValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
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
    SSADoWithCleanupInstructionPtr doWithCleanUp(const SSACodeRegionPtr &bodyRegion, const SSACodeRegionPtr &cleanUpRegion);
    SSADoWhileInstructionPtr doWhileContinueWith(const SSACodeRegionPtr &bodyRegion, const SSACodeRegionPtr &conditionRegion, const SSACodeRegionPtr &continueRegion);
    SSAIfInstructionPtr ifTrueIfFalse(const TypePtr &resultType, const SSAValuePtr &condition, const SSACodeRegionPtr &trueRegion, const SSACodeRegionPtr &falseRegion);
    SSALoadInstructionPtr load(const SSAValuePtr &reference);
    SSALocalVariableInstructionPtr localVariable(const TypePtr &referenceType, const TypePtr &valueType);
    SSAMakeAggregateInstructionPtr makeAggregate(const TypePtr &aggregateType, const SSAValuePtrList &elements);
    SSAReturnFromFunctionInstructionPtr returnFromFunction(const SSAValuePtr &value);
    SSAReturnFromRegionInstructionPtr returnFromRegion(const SSAValuePtr &value);
    SSASendMessageInstructionPtr sendMessage(const TypePtr &resultType, const SSAValuePtr &selector, const SSAValuePtr &receiver, const SSAValuePtrList &arguments);
    SSAStoreInstructionPtr storeValueIn(const SSAValuePtr &value, const SSAValuePtr &reference);
    SSAUnreachableInstructionPtr unreachableInstruction();
    SSAWhileInstructionPtr whileDoContinueWith(const SSACodeRegionPtr &conditionRegion, const SSACodeRegionPtr &bodyRegion, const SSACodeRegionPtr &continueRegion);

    SSABitcastInstructionPtr bitcast(const TypePtr &targetType, const SSAValuePtr &value);
    SSAUpcastInstructionPtr upcast(const TypePtr &targetType, const SSAValuePtr &value);
    SSADowncastInstructionPtr downcast(const TypePtr &targetType, const SSAValuePtr &value);
protected:
    SSACodeRegionPtr currentRegion;
    SSABasicBlockPtr currentBasicBlock;
    SSAInstructionPtr currentPosition;
    ASTSourcePositionPtr currentSourcePosition;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_BUILDER_HPP