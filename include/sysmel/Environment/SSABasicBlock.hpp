#ifndef SYSMEL_ENVIRONMENT_SSA_BASIC_BLOCK_HPP
#define SYSMEL_ENVIRONMENT_SSA_BASIC_BLOCK_HPP
#pragma once

#include "SSAValue.hpp"

namespace Sysmel
{
namespace Environment
{

typedef std::function<void (SSAInstructionPtr)> SSAInstructionIterationBlock;

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSABasicBlock : public SubtypeOf<SSAValue, SSABasicBlock>
{
public:
    static constexpr char const __typeName__[] = "SSABasicBlock";

    virtual bool isSSABasicBlock() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;

    const SSACodeRegionPtr &getParentCodeRegion() const;
    void setParentCodeRegion(const SSACodeRegionPtr &newParent);

    const SSAInstructionPtr &getFirstInstruction() const;
    const SSAInstructionPtr &getLastInstruction() const;

    void instructionsDo(const SSAInstructionIterationBlock &aBlock) const;
    void instructionsDo(const SSAInstructionIterationBlock &aBlock);

    void addInstructionBefore(const SSAInstructionPtr &instruction, const SSAInstructionPtr &position);
    void addInstructionAfter(const SSAInstructionPtr &instruction, const SSAInstructionPtr &position);
    void prependInstruction(const SSAInstructionPtr &instruction);
    void appendInstruction(const SSAInstructionPtr &instruction);
    void removeInstruction(const SSAInstructionPtr &instruction);

    void enumerateLocalValues(struct SSACodeRegionLocalValueEnumerationState &state);

protected:
    uint32_t basicBlockIndex = 0;

    SSAInstructionPtr firstInstruction;
    SSAInstructionPtr lastInstruction;
    SSACodeRegionPtr parentCodeRegion;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_BASIC_BLOCK_HPP