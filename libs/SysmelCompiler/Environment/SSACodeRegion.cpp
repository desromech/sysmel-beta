#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSACodeRegionArgument.hpp"
#include "Environment/SSACodeRegionCapture.hpp"
#include "Environment/SSABasicBlock.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSACodeRegion> SSACodeRegionTypeRegistration;

SSACodeRegion::SSACodeRegion()
{
    sourcePosition = ASTSourcePosition::empty();
}

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
    declaredResultType = resultType = functionalType->getResultType();
    if(resultType->isReturnedByReference())
    {
        addResultArgumentWithType(resultType->tempRef());
        resultType = Type::getVoidType();
    }

    addReceiverArgumentWithType(functionalType->getReceiverType());
    auto argumentCount = functionalType->getArgumentCount();
    for(size_t i = 0; i < argumentCount; ++i)
        addArgumentWithType(functionalType->getArgument(i));
}

void SSACodeRegion::setSignature(const TypePtrList &newArgumentTypes, const TypePtr &newResultType)
{
    arguments.clear();
    arguments.reserve(newArgumentTypes.size());
 
    declaredResultType = resultType = newResultType;
    if(resultType->isReturnedByReference())
    {
        addResultArgumentWithType(resultType->tempRef());
        resultType = Type::getVoidType();
    }

    for(auto &argType : newArgumentTypes)
        addArgumentWithType(argType);
}

void SSACodeRegion::addArgumentWithType(const TypePtr &argumentType)
{
    if(argumentType->isVoidType())
        return;
    
    if(argumentType->isPassedByReference())
    {
        arguments.push_back(SSACodeRegionArgument::make(argumentType->ref(), argumentType));
    }
    else
    {
        arguments.push_back(SSACodeRegionArgument::make(argumentType, argumentType));
    }
}

void SSACodeRegion::addReceiverArgumentWithType(const TypePtr &argumentType)
{
    if(argumentType->isVoidType())
        return;
    
    arguments.push_back(SSACodeRegionArgument::makeReceiver(argumentType));
}

void SSACodeRegion::addResultArgumentWithType(const TypePtr &argumentType)
{
    sysmelAssert(argumentType->isReferenceLikeType());
    arguments.push_back(SSACodeRegionArgument::makeResult(argumentType));
}

void SSACodeRegion::setSourcePosition(const ASTSourcePositionPtr &newSourcePosition)
{
    sourcePosition = newSourcePosition;
}

const ASTSourcePositionPtr &SSACodeRegion::getSourcePosition() const
{
    return sourcePosition;
}

const TypePtr &SSACodeRegion::getDeclaredResultType() const
{
    return declaredResultType;
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

bool SSACodeRegion::isReturningByReference() const
{
    return !arguments.empty() && arguments[0]->isResult();
}

bool SSACodeRegion::hasCVarArgs() const
{
    return !arguments.empty() && arguments.back()->getValueType()->isCVarArgsType();
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
    sysmelAssert(!captureType->isVoidType());
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