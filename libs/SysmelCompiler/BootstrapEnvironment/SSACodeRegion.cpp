#include "sysmel/BootstrapEnvironment/SSACodeRegion.hpp"
#include "sysmel/BootstrapEnvironment/SSACodeRegionArgument.hpp"
#include "sysmel/BootstrapEnvironment/SSABasicBlock.hpp"
#include "sysmel/BootstrapEnvironment/SSAValueVisitor.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourcePosition.hpp"
#include "sysmel/BootstrapEnvironment/FunctionalType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SSACodeRegion> SSACodeRegionTypeRegistration;

bool SSACodeRegion::isSSACodeRegion() const
{
    return true;
}

AnyValuePtr SSACodeRegion::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitCodeRegion(selfFromThis());
}

void SSACodeRegion::setFunctionalType(const FunctionalTypePtr &functionalType)
{
    addArgumentWithType(functionalType->getReceiverType());
    auto argumentCount = functionalType->getArgumentCount();
    for(size_t i = 0; i < argumentCount; ++i)
        addArgumentWithType(functionalType->getArgument(i));

    resultType = functionalType->getResultType();
}


void SSACodeRegion::setSignature(const TypePtrList &newArgumentTypes, const TypePtr &newResultType)
{
    arguments.clear();
    arguments.reserve(newArgumentTypes.size());
    for(auto &argType : newArgumentTypes)
        addArgumentWithType(argType);
    resultType = newResultType;
}

void SSACodeRegion::addArgumentWithType(const TypePtr &argumentType)
{
    if(argumentType->isVoidType())
        return;
    
    arguments.push_back(SSACodeRegionArgument::make(argumentType));
}

void SSACodeRegion::setSourcePosition(const ASTSourcePositionPtr &newSourcePosition)
{
    sourcePosition = newSourcePosition;
}

const TypePtr &SSACodeRegion::getResultType() const
{
    return resultType;
}

size_t SSACodeRegion::getArgumentCount() const
{
    return arguments.size();
}

const SSACodeRegionArgumentPtr &SSACodeRegion::getArgument(size_t index)
{
    return arguments[index];
}

const SSACodeRegionArgumentPtrList &SSACodeRegion::getArguments()
{
    return arguments;
}

const SSABasicBlockPtrList &SSACodeRegion::getBasicBlocks()
{
    return basicBlocks;
}

SExpression SSACodeRegion::asFullSExpression() const
{
    SExpressionList argumentsSExpr;
    argumentsSExpr.elements.reserve(basicBlocks.size());
    for(auto &arg : arguments)
        argumentsSExpr.elements.push_back(arg->asFullSExpression());

    SExpressionList basicBlocksSExpr;
    basicBlocksSExpr.elements.reserve(basicBlocks.size());
    for(auto &bb : basicBlocks)
        basicBlocksSExpr.elements.push_back(bb->asFullSExpression());

    return SExpressionList{{SExpressionIdentifier{{"region"}},
        sourcePosition ? sourcePosition->asSExpression() : nullptr,
        argumentsSExpr,
        resultType->asSExpression(),
        basicBlocksSExpr
    }};
}

SSABasicBlockPtr SSACodeRegion::newBasicBlock()
{
    auto basicBlock = basicMakeObject<SSABasicBlock> ();
    addBasicBlock(basicBlock);
    return basicBlock;
}

void SSACodeRegion::addBasicBlock(const SSABasicBlockPtr &block)
{
    block->setParentCodeRegion(selfFromThis());
    basicBlocks.push_back(block);
}

void SSACodeRegion::enumerateLocalValues(struct SSACodeRegionLocalValueEnumerationState &state)
{
    for(auto &arg : arguments)
        arg->enumerateLocalValues(state);
        
    for(auto &bb : basicBlocks)
        bb->enumerateLocalValues(state);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius