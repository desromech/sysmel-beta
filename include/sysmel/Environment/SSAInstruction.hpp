#ifndef SYSMEL_ENVIRONMENT_SSA_INSTRUCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_INSTRUCTION_HPP
#pragma once

#include "SSACodeRegionLocalValue.hpp"

namespace Sysmel
{
namespace Environment
{

typedef std::function<void (SSAValuePtr&)> SSAInstructionParameterIterationBlock;
typedef std::function<void (const SSAValuePtr&)> SSAInstructionConstParameterIterationBlock;
typedef std::function<void (const SSACodeRegionPtr&)> SSAInstructionRegionIterationBlock;

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSAInstruction : public SubtypeOf<SSACodeRegionLocalValue, SSAInstruction>
{
public:
    static constexpr char const __typeName__[] = "SSAInstruction";

    virtual bool isSSAInstruction() const override;

    virtual void enumerateLocalValues(SSACodeRegionLocalValueEnumerationState &state) override;

    virtual std::string getMnemonic() const;

    virtual void parametersDo(const SSAInstructionParameterIterationBlock &aBlock);
    virtual void parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const;

    virtual void regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const;

    virtual void replaceUsesOfWith(const SSAValuePtr &useToReplace, const SSAValuePtr &replacement) override;
    virtual void replaceWith(const SSAValuePtr &replacement) override;

    virtual SExpression asFullDefinitionSExpression() const override;

    const SSABasicBlockPtr &getParentBasicBlock() const;
    void setParentBasicBlock(const SSABasicBlockPtr &newParent);

    const SSAInstructionPtr &getPreviousInstruction() const;
    void setPreviousInstruction(const SSAInstructionPtr &instruction);

    const SSAInstructionPtr &getNextInstruction() const;
    void setNextInstruction(const SSAInstructionPtr &instruction);

    const ASTSourcePositionPtr &getSourcePosition() const;
    void setSourcePosition(const ASTSourcePositionPtr &newSourcePosition);

protected:

    ASTSourcePositionPtr sourcePosition;

    SSABasicBlockPtr parent;
    SSAInstructionPtr previousInstruction;
    SSAInstructionPtr nextInstruction;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_INSTRUCTION_HPP