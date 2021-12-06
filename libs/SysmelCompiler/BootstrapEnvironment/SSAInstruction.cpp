#include "sysmel/BootstrapEnvironment/SSAInstruction.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/SSACodeRegion.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSAInstruction> SSAInstructionTypeRegistration;

bool SSAInstruction::isSSAInstruction() const
{
    return true;
}

void SSAInstruction::enumerateLocalValues(SSACodeRegionLocalValueEnumerationState &state)
{
    SuperType::enumerateLocalValues(state);
    regionsDo([&](const SSACodeRegionPtr &region) {
        if(region)
            region->enumerateLocalValues(state);
    });
}

std::string SSAInstruction::getMnemonic() const
{
    SysmelSelfSubclassResponsibility();
}

void SSAInstruction::parametersDo(const SSAInstructionParameterIterationBlock &aBlock)
{
    (void)aBlock;
}

void SSAInstruction::parametersDo(const SSAInstructionConstParameterIterationBlock &aBlock) const
{
    (void)aBlock;
}

void SSAInstruction::regionsDo(const SSAInstructionRegionIterationBlock &aBlock) const
{
    (void)aBlock;
}

SExpression SSAInstruction::asFullDefinitionSExpression() const
{
    auto result = SExpressionList{{SExpressionIdentifier{{getMnemonic()}},
        LargeInteger{localValueIndex},
        getValueType()->asSExpression(),
        sourcePosition ? sourcePosition->asSExpression() : nullptr,
    }};

    parametersDo([&](const SSAValuePtr &param) {
        result.elements.push_back(param ? param->asSExpression() : nullptr);
    });

    regionsDo([&](const SSACodeRegionPtr &region) {
        result.elements.push_back(region ? region->asFullDefinitionSExpression() : nullptr);
    });

    return result;
}

const SSABasicBlockPtr &SSAInstruction::getParentBasicBlock() const
{
    return parent;
}

void SSAInstruction::setParentBasicBlock(const SSABasicBlockPtr &newParent)
{
    parent = newParent;
}

const SSAInstructionPtr &SSAInstruction::getPreviousInstruction() const
{
    return previousInstruction;
}

void SSAInstruction::setPreviousInstruction(const SSAInstructionPtr &instruction)
{
    previousInstruction = instruction;
}

const SSAInstructionPtr &SSAInstruction::getNextInstruction() const
{
    return nextInstruction;
}

void SSAInstruction::setNextInstruction(const SSAInstructionPtr &instruction)
{
    nextInstruction = instruction;
}

const ASTSourcePositionPtr &SSAInstruction::getSourcePosition() const
{
    return sourcePosition;
}

void SSAInstruction::setSourcePosition(const ASTSourcePositionPtr &newSourcePosition)
{
    sourcePosition = newSourcePosition;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius