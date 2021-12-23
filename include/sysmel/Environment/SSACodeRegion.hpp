#ifndef SYSMEL_ENVIRONMENT_SSA_CODE_REGION_HPP
#define SYSMEL_ENVIRONMENT_SSA_CODE_REGION_HPP
#pragma once

#include "SSAValue.hpp"

namespace Sysmel
{
namespace Environment
{

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSACodeRegion : public SubtypeOf<SSAValue, SSACodeRegion>
{
public:
    static constexpr char const __typeName__[] = "SSACodeRegion";

    virtual bool isSSACodeRegion() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
    virtual SExpression asFullDefinitionSExpression() const override;

    void setFunctionalType(const FunctionalTypePtr &functionalType);
    void setSignature(const TypePtrList &newArgumentTypes, const TypePtr &newResultType);
    void setSourcePosition(const ASTSourcePositionPtr &newSourcePosition);

    const TypePtr &getResultType() const;

    size_t getArgumentCount() const;
    const SSACodeRegionArgumentPtr &getArgument(size_t index);
    const SSACodeRegionArgumentPtrList &getArguments();
    bool isReturningByReference() const;

    size_t getCaptureCount() const;
    const SSACodeRegionCapturePtr &getCapture(size_t index);
    const SSACodeRegionCapturePtrList &getCaptures();
    SSACodeRegionCapturePtr addCaptureWithType(const TypePtr &captureType);

    bool isEmpty() const;
    const SSABasicBlockPtrList &getBasicBlocks();

    SSABasicBlockPtr newBasicBlock();
    void addBasicBlock(const SSABasicBlockPtr &block);

    void enumerateLocalValues(struct SSACodeRegionLocalValueEnumerationState &state);
protected:
    void addArgumentWithType(const TypePtr &argumentType);
    void addReceiverArgumentWithType(const TypePtr &argumentType);
    void addResultArgumentWithType(const TypePtr &argumentType);
    
    ASTSourcePositionPtr sourcePosition;
    SSACodeRegionArgumentPtrList arguments;
    SSACodeRegionCapturePtrList captures;
    TypePtr resultType;
    SSABasicBlockPtrList basicBlocks;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_CODE_REGION_HPP