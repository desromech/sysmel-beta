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
    instructionsSExpr.elements.reserve(instructions.size());
    for(auto &instr : instructions)
        instructionsSExpr.elements.push_back(instr->asFullSExpression());

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
    assert(!parentCodeRegion);
    parentCodeRegion = newParent;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius