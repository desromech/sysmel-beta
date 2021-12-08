#ifndef SYSMEL_ENVIRONMENT_SSA_CODE_REGION_ARGUMENT_HPP
#define SYSMEL_ENVIRONMENT_SSA_CODE_REGION_ARGUMENT_HPP
#pragma once

#include "SSACodeRegionLocalValue.hpp"

namespace Sysmel
{
namespace Environment
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
    virtual SExpression asFullDefinitionSExpression() const override;

    void setDeclarationPosition(const ASTSourcePositionPtr &position);
    void setDefinitionPosition(const ASTSourcePositionPtr &position);
    
    virtual TypePtr getValueType() const override;

protected:
    TypePtr valueType;
    ASTSourcePositionPtr declarationSourcePosition;
    ASTSourcePositionPtr definitionSourcePosition;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_CODE_REGION_ARGUMENT_HPP