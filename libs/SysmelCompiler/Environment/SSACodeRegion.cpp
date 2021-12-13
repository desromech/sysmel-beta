#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSACodeRegionArgument.hpp"
#include "Environment/SSACodeRegionCapture.hpp"
#include "Environment/SSABasicBlock.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

bool SSACodeRegion::isEmpty() const
{
    return basicBlocks.empty();
}

const SSABasicBlockPtrList &SSACodeRegion::getBasicBlocks()
{
    return basicBlocks;
}

size_t SSACodeRegion::getCaptureCount() const
{
    return captures.size();
}

const SSACodeRegionCapturePtr &SSACodeRegion::getCapture(size_t index)
{
    return captures[index];
}

const SSACodeRegionCapturePtrList &SSACodeRegion::getCaptures()
{
    return captures;
}

SSACodeRegionCapturePtr SSACodeRegion::addCaptureWithType(const TypePtr &captureType)
{
    assert(!captureType->isVoidType());
    auto result = SSACodeRegionCapture::make(captureType);
    captures.push_back(result);
    return result;
}

SExpression SSACodeRegion::asFullDefinitionSExpression() const
{
    SExpressionList argumentsSExpr;
    argumentsSExpr.elements.reserve(basicBlocks.size());
    for(auto &arg : arguments)
        argumentsSExpr.elements.push_back(arg->asFullDefinitionSExpression());

    SExpressionList capturesSExpr;
    capturesSExpr.elements.reserve(basicBlocks.size());
    for(auto &capture : captures)
        capturesSExpr.elements.push_back(capture->asFullDefinitionSExpression());

    SExpressionList basicBlocksSExpr;
    basicBlocksSExpr.elements.reserve(basicBlocks.size());
    for(auto &bb : basicBlocks)
        basicBlocksSExpr.elements.push_back(bb->asFullDefinitionSExpression());

    return SExpressionList{{SExpressionIdentifier{{"region"}},
        sourcePosition ? sourcePosition->asSExpression() : nullptr,
        argumentsSExpr,
        capturesSExpr,
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

    for(auto &capture : captures)
        capture->enumerateLocalValues(state);

    for(auto &bb : basicBlocks)
        bb->enumerateLocalValues(state);
}

} // End of namespace Environment
} // End of namespace Sysmel