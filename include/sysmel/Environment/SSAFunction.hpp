#ifndef SYSMEL_ENVIRONMENT_SSA_FUNCTION_HPP
#define SYSMEL_ENVIRONMENT_SSA_FUNCTION_HPP
#pragma once

#include "SSAProgramEntityWithChildren.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ProgramEntity);

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SYSMEL_COMPILER_LIB_EXPORT SSAFunction : public SubtypeOf<SSAProgramEntityWithChildren, SSAFunction>
{
public:
    static constexpr char const __typeName__[] = "SSAFunction";

    virtual bool isSSAFunction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;

    virtual TypePtr getValueType() const override;
    const FunctionalTypePtr &getFunctionalType();

    void initializeWithNameAndType(const AnyValuePtr &newName, const FunctionalTypePtr &newFunctionalType);
    void setDeclarationPosition(const ASTSourcePositionPtr &position);
    void setDefinitionPosition(const ASTSourcePositionPtr &position);

    SSACodeRegionPtr getMainCodeRegion() const;

    ASTSourcePositionPtr getDeclarationSourcePosition() const;
    ASTSourcePositionPtr getDefinitionSourcePosition() const;

    const AnyValuePtr &getIntrinsicName() const;
    void setIntrinsicName(const AnyValuePtr &newIntrinsicName);

    const ProgramEntityPtr &getSourceProgramEntity();
    void setSourceProgramEntity(const ProgramEntityPtr &entity);

protected:
    void enumerateLocalValues() const;

    AnyValuePtr intrinsicName;
    FunctionalTypePtr functionalType;
    ASTSourcePositionPtr sourcePosition;
    ProgramEntityPtr sourceProgramEntity;

    SSACodeRegionPtr mainCodeRegion;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_FUNCTION_HPP