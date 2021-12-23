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

    static SSACodeRegionArgumentPtr make(const TypePtr &valueType, const TypePtr &sourceType);
    static SSACodeRegionArgumentPtr makeReceiver(const TypePtr &valueType);
    static SSACodeRegionArgumentPtr makeResult(const TypePtr &valueType);

    virtual bool isSSACodeRegionArgument() const override;

    virtual AnyValuePtr accept(const SSAValueVisitorPtr &visitor) override;
    virtual SExpression asFullDefinitionSExpression() const override;

    void setDeclarationPosition(const ASTSourcePositionPtr &position);
    void setDefinitionPosition(const ASTSourcePositionPtr &position);
    
    virtual TypePtr getValueType() const override;

    bool isResult() const
    {
        return isResult_;
    }

    bool isReceiver() const
    {
        return isReceiver_;
    }

    bool isLocalFinalizationRequired() const;
    virtual void markLocalFinalizationRequired() override;

    const TypePtr &getSourceType() const;
    void setSourceType(const TypePtr &newSourceType);

protected:
    TypePtr valueType;
    TypePtr sourceType;
    ASTSourcePositionPtr declarationSourcePosition;
    ASTSourcePositionPtr definitionSourcePosition;
    bool isResult_  = false;
    bool isReceiver_ = false;
    bool localFinalizationRequired = false;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_SSA_CODE_REGION_ARGUMENT_HPP