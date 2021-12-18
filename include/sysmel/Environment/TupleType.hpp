#ifndef SYSMEL_ENVIRONMENT_TUPLE_TYPE_HPP
#define SYSMEL_ENVIRONMENT_TUPLE_TYPE_HPP
#pragma once

#include "AggregateType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(TupleType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(TupleTypeValue);

/**
 * I am an instance of a function type object.
 */
class TupleType : public SubMetaTypeOf<AggregateType, TupleType>
{
public:
    static TypePtr make(const TypePtrList &elementTypes);
    static TupleTypePtr makeNormalized(const TypePtrList &elementTypes);

    virtual bool isTupleType() const override;
    
    virtual bool supportsDynamicCompileTimeMessageSend() const override;
    
    virtual bool isNullableType() const override;

    virtual AnyValuePtr basicNewValue() override;
    TupleTypeValuePtr makeWithElements(const AnyValuePtrList &elements);

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    virtual TypePtr asTupleType() override;
    virtual TypePtr appendTypeMakingTuple(const TypePtr &nextType) override;
    virtual TypePtr appendResultTypeMakingFunctionType(const TypePtr &nextType) override;

    void addSpecializedInstanceMethods();

    TypePtrList elementTypes;

protected:
    virtual void buildLayout() override;
};

/**
 * I am an instance of a function type object.
 */
class TupleTypeValue : public SubtypeOf<AggregateTypeValue, TupleTypeValue>
{
public:
    static constexpr char const __typeName__[] = "TupleType";
    static constexpr char const __sysmelTypeName__[] = "_TupleType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isTupleTypeValue() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
    virtual TypePtr getType() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    AnyValuePtrList elements;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_STRUCTURE_TYPE_HPP