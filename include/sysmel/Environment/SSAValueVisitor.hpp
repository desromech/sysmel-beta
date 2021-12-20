#ifndef SYSMEL_ENVIRONMENT_SSA_VALUE_VISITOR_HPP
#define SYSMEL_ENVIRONMENT_SSA_VALUE_VISITOR_HPP
#pragma once

#include "SSAValue.hpp"

namespace Sysmel
{
namespace Environment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAValueVisitor : public SubtypeOf<CompilerObject, SSAValueVisitor>
{
public:
    static constexpr char const __typeName__[] = "SSAValueVisitor";

    virtual AnyValuePtr visitValue(const SSAValuePtr &value);
    virtual AnyValuePtr visitConstantValue(const SSAConstantValuePtr &value);
    virtual AnyValuePtr visitConstantLiteralValue(const SSAConstantLiteralValuePtr &value);
    virtual AnyValuePtr visitGlobalValue(const SSAGlobalValuePtr &value);
    virtual AnyValuePtr visitProgramEntity(const SSAProgramEntityPtr &value);
    virtual AnyValuePtr visitModule(const SSAModulePtr &value);
    virtual AnyValuePtr visitGlobalVariable(const SSAGlobalVariablePtr &value);
    virtual AnyValuePtr visitProgramEntityWithChildren(const SSAProgramEntityWithChildrenPtr &value);
    virtual AnyValuePtr visitFunction(const SSAFunctionPtr &value);
    virtual AnyValuePtr visitNamespace(const SSANamespacePtr &value);
    virtual AnyValuePtr visitTypeProgramEntity(const SSATypeProgramEntityPtr &value);
    virtual AnyValuePtr visitTemplate(const SSATemplatePtr &value);
    virtual AnyValuePtr visitTemplateInstance(const SSATemplateInstancePtr &value);

    virtual AnyValuePtr visitCodeRegion(const SSACodeRegionPtr &value);
    virtual AnyValuePtr visitBasicBlock(const SSABasicBlockPtr &value);

    virtual AnyValuePtr visitCodeRegionLocalValue(const SSACodeRegionLocalValuePtr &value);
    virtual AnyValuePtr visitCodeRegionArgument(const SSACodeRegionArgumentPtr &value);
    virtual AnyValuePtr visitCodeRegionCapture(const SSACodeRegionCapturePtr &value);

    virtual AnyValuePtr visitInstruction(const SSAInstructionPtr &instruction);
    virtual AnyValuePtr visitBreakInstruction(const SSABreakInstructionPtr &instruction);
    virtual AnyValuePtr visitCallInstruction(const SSACallInstructionPtr &instruction);
    virtual AnyValuePtr visitContinueInstruction(const SSAContinueInstructionPtr &instruction);
    virtual AnyValuePtr visitConditionalJumpInstruction(const SSAConditionalJumpInstructionPtr &instruction);
    virtual AnyValuePtr visitIfInstruction(const SSAIfInstructionPtr &instruction);
    virtual AnyValuePtr visitJumpInstruction(const SSAJumpInstructionPtr &instruction);
    virtual AnyValuePtr visitLoadInstruction(const SSALoadInstructionPtr &instruction);
    virtual AnyValuePtr visitLocalVariableInstruction(const SSALocalVariableInstructionPtr &instruction);
    virtual AnyValuePtr visitDoWhileInstruction(const SSADoWhileInstructionPtr &instruction);
    virtual AnyValuePtr visitDoWithCleanupInstruction(const SSADoWithCleanupInstructionPtr &instruction);
    virtual AnyValuePtr visitGetAggregateFieldReferenceInstruction(const SSAGetAggregateFieldReferenceInstructionPtr &instruction);
    virtual AnyValuePtr visitGetAggregateSlotReferenceInstruction(const SSAGetAggregateSlotReferenceInstructionPtr &instruction);
    virtual AnyValuePtr visitMakeAggregateInstruction(const SSAMakeAggregateInstructionPtr &instruction);
    virtual AnyValuePtr visitMakeClosureInstruction(const SSAMakeClosureInstructionPtr &instruction);
    virtual AnyValuePtr visitReturnFromRegionInstruction(const SSAReturnFromRegionInstructionPtr &instruction);
    virtual AnyValuePtr visitReturnFromFunctionInstruction(const SSAReturnFromFunctionInstructionPtr &instruction);
    virtual AnyValuePtr visitSendMessageInstruction(const SSASendMessageInstructionPtr &instruction);
    virtual AnyValuePtr visitStoreInstruction(const SSAStoreInstructionPtr &instruction);
    virtual AnyValuePtr visitUnreachableInstruction(const SSAUnreachableInstructionPtr &instruction);
    virtual AnyValuePtr visitWhileInstruction(const SSAWhileInstructionPtr &instruction);

    virtual AnyValuePtr visitCastInstruction(const SSACastInstructionPtr &instruction);
    virtual AnyValuePtr visitUpcastInstruction(const SSAUpcastInstructionPtr &instruction);
    virtual AnyValuePtr visitDowncastInstruction(const SSADowncastInstructionPtr &instruction);
    virtual AnyValuePtr visitBitcastInstruction(const SSABitcastInstructionPtr &instruction);

    virtual AnyValuePtr visitEnableLocalFinalization(const SSAEnableLocalFinalizationPtr &instruction);
    virtual AnyValuePtr visitLocalFinalization(const SSALocalFinalizationPtr &instruction);
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_VALUE_VISITOR_HPP