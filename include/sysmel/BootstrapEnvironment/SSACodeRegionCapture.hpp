#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CODE_REGION_CAPTURE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CODE_REGION_CAPTURE_HPP
#pragma once

#include "SSACodeRegionLocalValue.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
/**
 * I am the base interface for any object is specifically defined in the compiler domain.
 */
class SSACodeRegionCapture : public SubtypeOf<SSACodeRegionLocalValue, SSACodeRegionCapture>
{
public:
    static constexpr char const __typeName__[] = "SSACodeRegionCapture";

    static SSACodeRegionCapturePtr make(const TypePtr &valueType);

    virtual bool isSSACodeRegionCapture() const override;

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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_SSA_CODE_REGION_CAPTURE_HPP