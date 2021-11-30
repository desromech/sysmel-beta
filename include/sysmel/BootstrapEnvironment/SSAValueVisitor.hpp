#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_VALUE_VISITOR_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_VALUE_VISITOR_HPP
#pragma once

#include "SSAValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
    virtual AnyValuePtr visitGlobalVariable(const SSAGlobalVariablePtr &value);
    virtual AnyValuePtr visitFunction(const SSAFunctionPtr &value);
    virtual AnyValuePtr visitCodeRegion(const SSACodeRegionPtr &value);
    virtual AnyValuePtr visitBasicBlock(const SSABasicBlockPtr &value);

    virtual AnyValuePtr visitCodeRegionLocalValue(const SSACodeRegionLocalValuePtr &value);
    virtual AnyValuePtr visitCodeRegionArgument(const SSACodeRegionArgumentPtr &value);

    virtual AnyValuePtr visitInstruction(const SSAInstructionPtr &instruction);
    virtual AnyValuePtr visitCallInstruction(const SSACallInstructionPtr &instruction);
    virtual AnyValuePtr visitConditionalJumpInstruction(const SSAConditionalJumpInstructionPtr &instruction);
    virtual AnyValuePtr visitIfInstruction(const SSAIfInstructionPtr &instruction);
    virtual AnyValuePtr visitJumpInstruction(const SSAJumpInstructionPtr &instruction);
    virtual AnyValuePtr visitDoWhileInstruction(const SSADoWhileInstructionPtr &instruction);
    virtual AnyValuePtr visitDoWithCleanupInstruction(const SSADoWithCleanupInstructionPtr &instruction);
    virtual AnyValuePtr visitMakeClosureInstruction(const SSAMakeClosureInstructionPtr &instruction);
    virtual AnyValuePtr visitReturnFromRegionInstruction(const SSAReturnFromRegionInstructionPtr &instruction);
    virtual AnyValuePtr visitReturnFromFunctionInstruction(const SSAReturnFromFunctionInstructionPtr &instruction);
    virtual AnyValuePtr visitSendMessageInstruction(const SSASendMessageInstructionPtr &instruction);
    virtual AnyValuePtr visitUnreachableInstruction(const SSAUnreachableInstructionPtr &instruction);
    virtual AnyValuePtr visitWhileInstruction(const SSAWhileInstructionPtr &instruction);
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_VALUE_VISITOR_HPP