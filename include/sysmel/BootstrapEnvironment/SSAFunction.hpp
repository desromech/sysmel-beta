#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_FUNCTION_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_FUNCTION_HPP
#pragma once

#include "SSAGlobalValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ProgramEntity);

/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSAFunction : public SubtypeOf<SSAGlobalValue, SSAFunction>
{
public:
    static constexpr char const __typeName__[] = "SSAFunction";

    virtual bool isSSAFunction() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;

    virtual AnyValuePtr getName() const override;
    virtual TypePtr getValueType() const override;
    const FunctionalTypePtr &getFunctionalType();

    void initializeWithNameAndType(const AnyValuePtr &newName, const FunctionalTypePtr &newFunctionalType);
    void setDeclarationPosition(const ASTSourcePositionPtr &position);
    void setDefinitionPosition(const ASTSourcePositionPtr &position);

    SSACodeRegionPtr getMainCodeRegion() const;

    const AnyValuePtr &getIntrinsicName() const;
    void setIntrinsicName(const AnyValuePtr &newIntrinsicName);

    const ProgramEntityPtr &getSourceProgramEntity();
    void setSourceProgramEntity(const ProgramEntityPtr &entity);

protected:
    void enumerateLocalValues() const;

    AnyValuePtr name;
    AnyValuePtr intrinsicName;
    FunctionalTypePtr functionalType;
    ASTSourcePositionPtr sourcePosition;
    ProgramEntityPtr sourceProgramEntity;

    SSACodeRegionPtr mainCodeRegion;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_FUNCTION_HPP