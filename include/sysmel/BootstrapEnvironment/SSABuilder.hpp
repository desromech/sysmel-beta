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

    SSABasicBlockPtr makeBasicBlock();
    SSABasicBlockPtr makeBasicBlockHere();

    void addInstruction(const SSAInstructionPtr &instruction);

    SSAValuePtr normalizeValue(const SSAValuePtr &value);

    SSAConstantLiteralValuePtr literalWithType(const AnyValuePtr &value, const TypePtr &type);
    SSAConstantLiteralValuePtr literal(const AnyValuePtr &value);

    SSACallInstructionPtr call(const TypePtr &resultType, const SSAValuePtr &function, const SSAValuePtrList &arguments);
    SSAReturnFromFunctionInstructionPtr returnFromFunction(const SSAValuePtr &value);
    SSAReturnFromRegionInstructionPtr returnFromRegion(const SSAValuePtr &value);
    SSASendMessageInstructionPtr sendMessage(const TypePtr &resultType, const SSAValuePtr &selector, const SSAValuePtr &receiver, const SSAValuePtrList &arguments);

protected:
    SSACodeRegionPtr currentRegion;
    SSABasicBlockPtr currentBasicBlock;
    SSAInstructionPtr currentPosition;
    ASTSourcePositionPtr currentSourcePosition;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_BUILDER_HPP