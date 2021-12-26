#include "Environment/SSAValueVisitor.hpp"
#include "Environment/SSAUpcastInstruction.hpp"
#include "Environment/SSADowncastInstruction.hpp"
#include "Environment/SSABitcastInstruction.hpp"

#include "Environment/SSATypeProgramEntity.hpp"
#include "Environment/SSATemplate.hpp"
#include "Environment/SSATemplateInstance.hpp"

#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

#include "Environment/SSANamespace.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAValueVisitor> SSAValueVisitorTypeRegistration;

AnyValuePtr SSAValueVisitor::visitValue(const SSAValuePtr &value)
{
    return value->accept(selfFromThis());
}

AnyValuePtr SSAValueVisitor::visitConstantValue(const SSAConstantValuePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitConstantLiteralValue(const SSAConstantLiteralValuePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitGlobalValue(const SSAGlobalValuePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitProgramEntity(const SSAProgramEntityPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitProgramEntityWithChildren(const SSAProgramEntityWithChildrenPtr &value)
{
    return visitProgramEntity(value);
}

AnyValuePtr SSAValueVisitor::visitModule(const SSAModulePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitGlobalVariable(const SSAGlobalVariablePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitFunction(const SSAFunctionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitNamespace(const SSANamespacePtr &value)
{
    return visitProgramEntityWithChildren(value);
}

AnyValuePtr SSAValueVisitor::visitTypeProgramEntity(const SSATypeProgramEntityPtr &value)
{
    return visitProgramEntityWithChildren(value);
}

AnyValuePtr SSAValueVisitor::visitTemplate(const SSATemplatePtr &value)
{
    return visitProgramEntityWithChildren(value);
}

AnyValuePtr SSAValueVisitor::visitTemplateInstance(const SSATemplateInstancePtr &value)
{
    return visitProgramEntityWithChildren(value);
}

AnyValuePtr SSAValueVisitor::visitCodeRegion(const SSACodeRegionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitBasicBlock(const SSABasicBlockPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitCodeRegionLocalValue(const SSACodeRegionLocalValuePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitCodeRegionArgument(const SSACodeRegionArgumentPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitCodeRegionCapture(const SSACodeRegionCapturePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitInstruction(const SSAInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitBreakInstruction(const SSABreakInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitCallInstruction(const SSACallInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitContinueInstruction(const SSAContinueInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitConditionalJumpInstruction(const SSAConditionalJumpInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitIfInstruction(const SSAIfInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitJumpInstruction(const SSAJumpInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitLoadInstruction(const SSALoadInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitLocalVariableInstruction(const SSALocalVariableInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitDeclareLocalVariableInstruction(const SSADeclareLocalVariableInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitDoWhileInstruction(const SSADoWhileInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitDoWithCleanupInstruction(const SSADoWithCleanupInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitGetAggregateFieldReferenceInstruction(const SSAGetAggregateFieldReferenceInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitGetAggregateSlotReferenceInstruction(const SSAGetAggregateSlotReferenceInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitMakeClosureInstruction(const SSAMakeClosureInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitReturnFromRegionInstruction(const SSAReturnFromRegionInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitReturnFromFunctionInstruction(const SSAReturnFromFunctionInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitSendMessageInstruction(const SSASendMessageInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitStoreInstruction(const SSAStoreInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitUnreachableInstruction(const SSAUnreachableInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitWhileInstruction(const SSAWhileInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitCastInstruction(const SSACastInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitUpcastInstruction(const SSAUpcastInstructionPtr &instruction)
{
    return visitCastInstruction(instruction);
}

AnyValuePtr SSAValueVisitor::visitDowncastInstruction(const SSADowncastInstructionPtr &instruction)
{
    return visitCastInstruction(instruction);
}

AnyValuePtr SSAValueVisitor::visitBitcastInstruction(const SSABitcastInstructionPtr &instruction)
{
    return visitCastInstruction(instruction);
}

AnyValuePtr SSAValueVisitor::visitEnableLocalFinalization(const SSAEnableLocalFinalizationPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitLocalFinalization(const SSALocalFinalizationPtr &)
{
    SysmelSelfSubclassResponsibility();
}

} // End of namespace Environment
} // End of namespace Sysmel