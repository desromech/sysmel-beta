#ifndef SYSMEL_ENVIRONMENT_METHOD_TYPE_HPP
#define SYSMEL_ENVIRONMENT_METHOD_TYPE_HPP
#pragma once

#include "FunctionalType.hpp"
#include "SpecificMethod.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(MethodType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(MethodTypeValue);

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT MethodType : public SubMetaTypeOf<FunctionalType, MethodType>
{
public:
    virtual bool isMethodType() const override;
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor) override;

    static MethodTypePtr make(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &arguments);

    virtual TypePtr getReceiverType() const override;
    virtual FunctionalTypeValuePtr makeValueWithImplementation(const AnyValuePtr &implementation) override;
    
    virtual std::string getQualifiedName() const override;
    virtual SExpression asSExpression() const override;

protected:
    TypePtr receiverType;
};

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT MethodTypeValue : public SubtypeOf<FunctionalTypeValue, MethodTypeValue>
{
public:
    static constexpr char const __typeName__[] = "MethodType";
    static constexpr char const __sysmelTypeName__[] = "_MethodType";

    virtual bool isMethodTypeValue() const override;

    virtual TypePtr getType() const override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_METHOD_TYPE_HPP