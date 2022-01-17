#include "Environment/SSAInstruction.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/SSABasicBlock.hpp"
#include "Environment/Type.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

void SSAInstruction::replaceUsesOfWith(const SSAValuePtr &useToReplace, const SSAValuePtr &replacement)
{
    SuperType::replaceUsesOfWith(useToReplace, replacement);
    parametersDo([&](SSAValuePtr &parameter) {
        if(parameter == useToReplace)
        {
            parameter = replacement;
            replacement->addUse(selfFromThis());
        }
    });
}

void SSAInstruction::replaceWith(const SSAValuePtr &replacement)
{
    SuperType::replaceWith(replacement);
    if(parent)
        parent->removeInstruction(selfFromThis());
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

const LexicalScopePtr &SSAInstruction::getLexicalScope() const
{
    return lexicalScope;
}

void SSAInstruction::setLexicalScope(const LexicalScopePtr &newLexicalScope)
{
    lexicalScope = newLexicalScope;
}

} // End of namespace Environment
} // End of namespace Sysmel