#ifndef SYSMEL_ENVIRONMENT_VARIANT_TYPE_HPP
#define SYSMEL_ENVIRONMENT_VARIANT_TYPE_HPP
#pragma once

#include "AggregateType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(VariantType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(VariantTypeValue);

/**
 * I am an instance of a function type object.
 */
class VariantType : public SubMetaTypeOf<AggregateType, VariantType>
{
public:
    static TypePtr make(const TypePtrList &elementTypes);
    static VariantTypePtr makeNormalized(const TypePtrList &elementTypes);

    virtual bool isVariantType() const override;
    
    virtual bool supportsDynamicCompileTimeMessageSend() const override;
    
    virtual bool isNullableType() const override;

    virtual AnyValuePtr basicNewValue() override;
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor) override;
    VariantTypeValuePtr makeWithElements(const AnyValuePtrList &elements);

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    virtual TypePtr appendTypeMakingVariant(const TypePtr &nextType) override;

    void addSpecializedInstanceMethods();

    TypePtrList elementTypes;

protected:
    virtual void buildLayout() override;
};

/**
 * I am an instance of a function type object.
 */
class VariantTypeValue : public SubtypeOf<AggregateTypeValue, VariantTypeValue>
{
public:
    static constexpr char const __typeName__[] = "VariantType";
    static constexpr char const __sysmelTypeName__[] = "_VariantType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isVariantTypeValue() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
    virtual TypePtr getType() const override;

    virtual std::string printString() const override;
    virtual SExpression asSExpression() const override;

    uint32_t typeIndex = 0;
    AnyValuePtr wrappedElement;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_STRUCTURE_TYPE_HPP