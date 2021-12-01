#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CODE_REGION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CODE_REGION_HPP
#pragma once

#include "SSAValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
    virtual SExpression asFullSExpression() const override;

    void setFunctionalType(const FunctionalTypePtr &functionalType);
    void setSignature(const TypePtrList &newArgumentTypes, const TypePtr &newResultType);
    void setSourcePosition(const ASTSourcePositionPtr &newSourcePosition);

    const TypePtr &getResultType() const;

    size_t getArgumentCount() const;
    const SSACodeRegionArgumentPtr &getArgument(size_t index);
    const SSACodeRegionArgumentPtrList &getArguments();

    const SSABasicBlockPtrList &getBasicBlocks();

    SSABasicBlockPtr newBasicBlock();
    void addBasicBlock(const SSABasicBlockPtr &block);

    void enumerateLocalValues(struct SSACodeRegionLocalValueEnumerationState &state);
protected:
    void addArgumentWithType(const TypePtr &argumentType);
    
    ASTSourcePositionPtr sourcePosition;
    SSACodeRegionArgumentPtrList arguments;
    TypePtr resultType;
    SSABasicBlockPtrList basicBlocks;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CODE_REGION_HPP