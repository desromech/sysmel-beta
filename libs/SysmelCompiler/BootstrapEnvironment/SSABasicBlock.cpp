#include "sysmel/BootstrapEnvironment/SSABasicBlock.hpp"
#include "sysmel/BootstrapEnvironment/SSAInstruction.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

SExpression SSABasicBlock::asFullSExpression() const
{
    SExpressionList instructionsSExpr;
    instructionsDo([&](const SSAInstructionPtr &instr) {
        instructionsSExpr.elements.push_back(instr->asFullSExpression());
    });

    return SExpressionList{{SExpressionIdentifier{{"basicBlock"}},
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius