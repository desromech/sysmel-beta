#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAValueVisitor> SSAValueVisitorTypeRegistration;

AnyValuePtr SSAValueVisitor::visitValue(const SSAValuePtr &)
{
    SysmelSelfSubclassResponsibility();
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

AnyValuePtr SSAValueVisitor::visitGlobalVariable(const SSAGlobalVariablePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitFunction(const SSAFunctionPtr &)
{
    SysmelSelfSubclassResponsibility();
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

AnyValuePtr SSAValueVisitor::visitDoWhileInstruction(const SSADoWhileInstructionPtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr SSAValueVisitor::visitDoWithCleanupInstruction(const SSADoWithCleanupInstructionPtr &)
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius