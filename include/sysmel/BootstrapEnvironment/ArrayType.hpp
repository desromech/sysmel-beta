#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ARRAY_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ARRAY_TYPE_HPP
#pragma once

#include "AggregateType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ArrayType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ArrayTypeValue);

/**
 * I am an instance of a function type object.
 */
class ArrayType : public SubMetaTypeOf<AggregateType, ArrayType>
{
public:
    static ArrayTypePtr make(const TypePtr &elementType, uint64_t size);
    
    virtual bool isArrayType() const override;
    
    virtual bool supportsDynamicCompileTimeMessageSend() const override;
    
    virtual bool isNullableType() const override;
    virtual bool isImmutableType() const override;
    virtual bool hasTrivialInitialization() const override;
    virtual bool hasTrivialFinalization() const override;
    virtual bool hasTrivialCopyingFrom() const override;
    virtual bool hasTrivialMovingFrom() const override;

    virtual AnyValuePtr basicNewValue() override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    void addSpecializedInstanceMethods();

    TypePtr elementType;
    uint64_t size;
};

/**
 * I am an instance of a function type object.
 */
class ArrayTypeValue : public SubtypeOf<AggregateTypeValue, ArrayTypeValue>
{
public:
    static constexpr char const __typeName__[] = "ArrayType";
    static constexpr char const __sysmelTypeName__[] = "_ArrayType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isArrayTypeValue() const override;
    virtual TypePtr getType() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    AnyValuePtrList elements;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ARRAY_TYPE_HPP