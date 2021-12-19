#ifndef SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_HPP
#define SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_HPP
#pragma once

#include "SimpleType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeLayout);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeValue);

/**
 * I am an instance of a function type object.
 */
class AggregateType : public SubMetaTypeOf<SimpleType, AggregateType>
{
public:
    virtual bool isAggregateType() const override;

    virtual uint64_t getMemorySize() override;
    virtual uint64_t getMemoryAlignment() override;

    virtual bool isImmutableType() override;
    virtual bool isPassedByReference() override;
    virtual bool isReturnedByReference() override;
    virtual TypePtr asConstReceiverType() override;
    virtual TypePtr asReceiverType() override;

    virtual bool hasTrivialInitialization() override;
    virtual bool hasTrivialInitializationCopyingFrom() override;
    virtual bool hasTrivialInitializationMovingFrom() override;
    virtual bool hasTrivialFinalization() override;
    virtual bool hasTrivialCopyingFrom() override;
    virtual bool hasTrivialMovingFrom() override;

    const AggregateTypeLayoutPtr &getLayout();

protected:
    virtual void buildLayout();

    AggregateTypeLayoutPtr layout;
};

/**
 * I am an instance of a function type object.
 */
class AggregateTypeValue : public SubtypeOf<AnyValue, AggregateTypeValue>
{
public:
    static constexpr char const __typeName__[] = "AggregateType";
    static constexpr char const __sysmelTypeName__[] = "_AggregateType";

    static constexpr bool __isDynamicCompileTimeType__ = false;
    static constexpr bool __isNullableType__ = false;

    virtual bool isAggregateTypeValue() const override;

    virtual AnyValuePtr asMutableStoreValue() override;
    virtual AnyValuePtr getReferenceToSlotWithType(int64_t slotIndex, int64_t slotOffset, const TypePtr &referenceType) override;

    virtual AnyValuePtr loadAggregateElement(int64_t slotIndex, int64_t slotOffset, const TypePtr &elementType) override;
    virtual AnyValuePtr copyAssignAggregateElement(int64_t slotIndex, int64_t slotOffset, const TypePtr &elementType, const AnyValuePtr &newValue) override;
    virtual AnyValuePtr moveAssignAggregateElement(int64_t slotIndex, int64_t slotOffset, const TypePtr &elementType, const AnyValuePtr &newValue) override;

    AggregateTypePtr type;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_FUNCTION_TYPE_HPP