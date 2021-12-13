#include "Environment/SSABasicBlock.hpp"
#include "Environment/SSAInstruction.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSABasicBlock> SSABasicBlockTypeRegistration;

bool SSABasicBlock::isSSABasicBlock() const
{
    return true;
}

AnyValuePtr SSABasicBlock::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitBasicBlock(selfFromThis());
}

SExpression SSABasicBlock::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"basicBlock"}},
        LargeInteger{basicBlockIndex},
    }};
}

SExpression SSABasicBlock::asFullDefinitionSExpression() const
{
    SExpressionList instructionsSExpr;
    instructionsDo([&](const SSAInstructionPtr &instr) {
        instructionsSExpr.elements.push_back(instr->asFullDefinitionSExpression());
    });

    return SExpressionList{{SExpressionIdentifier{{"basicBlock"}},
        LargeInteger{basicBlockIndex},
        instructionsSExpr
    }};
}

const SSACodeRegionPtr &SSABasicBlock::getParentCodeRegion() const
{
    return parentCodeRegion;
}

void SSABasicBlock::setParentCodeRegion(const SSACodeRegionPtr &newParent)
{
    parentCodeRegion = newParent;
}

const SSAInstructionPtr &SSABasicBlock::getFirstInstruction() const
{
    return firstInstruction;
}

const SSAInstructionPtr &SSABasicBlock::getLastInstruction() const
{
    return lastInstruction;
}

void SSABasicBlock::instructionsDo(const SSAInstructionIterationBlock &aBlock) const
{
    for(auto currentPosition = firstInstruction; currentPosition; )
    {
        auto oldPosition = currentPosition;
        currentPosition = currentPosition->getNextInstruction();
        aBlock(oldPosition);
    }
}

void SSABasicBlock::instructionsDo(const SSAInstructionIterationBlock &aBlock)
{
    for(auto currentPosition = firstInstruction; currentPosition; )
    {
        auto oldPosition = currentPosition;
        currentPosition = currentPosition->getNextInstruction();
        aBlock(oldPosition);
    }
}

void SSABasicBlock::addInstructionBefore(const SSAInstructionPtr &instruction, const SSAInstructionPtr &position)
{
    assert(!instruction->getParentBasicBlock());
    assert(!instruction->getPreviousInstruction());
    assert(!instruction->getNextInstruction());

    SSAInstructionPtr before = position ? position->getPreviousInstruction() : lastInstruction;
    SSAInstructionPtr after = position;

    if(before)
    {
        before->setNextInstruction(instruction);
        instruction->setPreviousInstruction(before);
    }
    else
    {
        firstInstruction = instruction;
    }

    if(after)
    {
        after->setPreviousInstruction(instruction);
        instruction->setNextInstruction(after);
    }
    else
    {
        lastInstruction = instruction;
    }
    
    instruction->setParentBasicBlock(selfFromThis());
}

void SSABasicBlock::addInstructionAfter(const SSAInstructionPtr &instruction, const SSAInstructionPtr &position)
{
    assert(!instruction->getParentBasicBlock());
    assert(!instruction->getPreviousInstruction());
    assert(!instruction->getNextInstruction());

    SSAInstructionPtr before = position;
    SSAInstructionPtr after = position ? position->getNextInstruction() : firstInstruction;

    if(before)
    {
        before->setNextInstruction(instruction);
        instruction->setPreviousInstruction(before);
    }
    else
    {
        firstInstruction = instruction;
    }

    if(after)
    {
        after->setPreviousInstruction(instruction);
        instruction->setNextInstruction(after);
    }
    else
    {
        lastInstruction = instruction;
    }
    
    instruction->setParentBasicBlock(selfFromThis());
}

void SSABasicBlock::prependInstruction(const SSAInstructionPtr &instruction)
{
    addInstructionAfter(instruction, nullptr);
}

void SSABasicBlock::appendInstruction(const SSAInstructionPtr &instruction)
{
    addInstructionBefore(instruction, nullptr);
}

void SSABasicBlock::removeInstruction(const SSAInstructionPtr &instruction)
{
    assert(instruction && instruction->getParentBasicBlock() == selfFromThis());
    SSAInstructionPtr before = instruction->getPreviousInstruction();
    SSAInstructionPtr after = instruction->getNextInstruction();

    if(before)
        before->setNextInstruction(after);
    else
        firstInstruction = after;

    if(after)
        after->setPreviousInstruction(before);
    else
        lastInstruction = before;
    
    instruction->setParentBasicBlock(nullptr);
    instruction->setPreviousInstruction(nullptr);
    instruction->setNextInstruction(nullptr);
}

void SSABasicBlock::enumerateLocalValues(struct SSACodeRegionLocalValueEnumerationState &state)
{
    basicBlockIndex = state.basicBlockCount++;

    instructionsDo([&](const SSAInstructionPtr &instruction) {
        instruction->enumerateLocalValues(state);
    });
}

} // End of namespace Environment
} // End of namespace Sysmel