#ifndef SYSMEL_ENVIRONMENT_ARRAY_TYPE_HPP
#define SYSMEL_ENVIRONMENT_ARRAY_TYPE_HPP
#pragma once

#include "AggregateType.hpp"

namespace Sysmel
{
namespace Environment
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
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor) override;
    
    virtual bool supportsDynamicCompileTimeMessageSend() const override;
    
    virtual bool isNullableType() const override;
    virtual bool isImmutableType() override;
    virtual bool hasTrivialInitialization() override;
    virtual bool hasTrivialInitializationCopyingFrom() override;
    virtual bool hasTrivialInitializationMovingFrom() override;
    virtual bool hasTrivialFinalization() override;
    virtual bool hasTrivialCopyingFrom() override;
    virtual bool hasTrivialMovingFrom() override;

    virtual uint64_t getMemorySize() override;
    virtual uint64_t getMemoryAlignment() override;
    
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
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
    virtual TypePtr getType() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    AnyValuePtrList elements;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_ARRAY_TYPE_HPP