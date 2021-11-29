#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CODE_REGION_ARGUMENT_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CODE_REGION_ARGUMENT_HPP
#pragma once

#include "SSACodeRegionLocalValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSACodeRegionArgument : public SubtypeOf<SSACodeRegionLocalValue, SSACodeRegionArgument>
{
public:
    static constexpr char const __typeName__[] = "SSACodeRegionArgument";

    static SSACodeRegionArgumentPtr make(const TypePtr &valueType);

    virtual bool isSSACodeRegionArgument() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
    virtual SExpression asFullSExpression() const override;

    void setDeclarationPosition(const ASTSourcePositionPtr &position);
    void setDefinitionPosition(const ASTSourcePositionPtr &position);
    
protected:
    TypePtr valueType;
    ASTSourcePositionPtr declarationSourcePosition;
    ASTSourcePositionPtr definitionSourcePosition;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CODE_REGION_ARGUMENT_HPP