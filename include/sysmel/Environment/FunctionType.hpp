#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_TYPE_HPP
#pragma once

#include "FunctionalType.hpp"
#include "SpecificMethod.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionTypeValue);

/**
 * I am an instance of a function type object.
 */
class FunctionType : public SubMetaTypeOf<FunctionalType, FunctionType>
{
public:
    static FunctionTypePtr make(const TypePtr &resultType, const TypePtrList &arguments);

    virtual bool isFunctionType() const override;

    virtual FunctionalTypeValuePtr makeValueWithImplementation(const AnyValuePtr &implementation) override;
    virtual FunctionTypePtr copyWithResultType(const TypePtr &newResultType) override;

    virtual SExpression asSExpression() const override;

    void addSpecializedMethods();
};

/**
 * I am an instance of a function type object.
 */
class FunctionTypeValue : public SubtypeOf<FunctionalTypeValue, FunctionTypeValue>
{
public:
    virtual bool isFunctionTypeValue() const override;

    virtual TypePtr getType() const override;
    
    virtual AnyValuePtr applyWithArguments(const std::vector<AnyValuePtr> &arguments) override;
    virtual SSAValuePtr asSSAValueRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition) override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_TYPE_HPP