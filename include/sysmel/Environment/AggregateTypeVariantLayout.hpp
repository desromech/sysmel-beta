#ifndef SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_VARIANT_LAYOUT_HPP
#define SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_VARIANT_LAYOUT_HPP
#pragma once

#include "AggregateTypeLayout.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeVariantLayout);


/**
 * I represent the layout of an aggregate type with single discriminated element.
 */
class SYSMEL_COMPILER_LIB_EXPORT AggregateTypeVariantLayout : public SubtypeOf<AggregateTypeLayout, AggregateTypeVariantLayout>
{
public:
    static constexpr char const __typeName__[] = "AggregateTypeVariantLayout";

    virtual uint64_t getSlotCount() const override;
    virtual uint64_t getMemorySize() override;
    virtual uint64_t getMemoryAlignment() override;

    virtual bool hasTrivialInitialization() override;
    virtual bool hasTrivialInitializationCopyingFrom() override;
    virtual bool hasTrivialInitializationMovingFrom() override;
    virtual bool hasTrivialFinalization() override;
    virtual bool hasTrivialAssignCopyingFrom() override;
    virtual bool hasTrivialAssignMovingFrom() override;

    virtual void beginGroup() override;
    virtual uint32_t addSlotWithType(const TypePtr &slotType) override;
    virtual void finishGroup() override;

    virtual TypePtr getTypeForSlotAndOffset(int64_t slotIndex, int64_t slotOffset) override;
    virtual TypePtr getTypeForNonPaddingSlot(int64_t slotIndex) override;

    virtual uint64_t getOffsetForSlotIndex(uint32_t slotIndex) override;

    const TypePtr &getDataTypeIndexType() const
    {
        return dataTypeIndexType;
    }

    uint64_t getPaddingSize() const
    {
        return paddingSize;
    }

    uint64_t getElementMemorySlotIndex() const
    {
        return paddingSize == 0 ? 1 : 2;
    }

    uint64_t getElementMemorySize() const
    {
        return elementMemorySize;
    }

    uint64_t getEndPaddingSize() const
    {
        return endPaddingSize;
    }

private:
    bool hasTrivialInitialization_ = true;
    bool hasTrivialInitializationCopyingFrom_ = true;
    bool hasTrivialInitializationMovingFrom_ = true;
    bool hasTrivialFinalization_ = true;
    bool hasTrivialAssignCopyingFrom_ = true;
    bool hasTrivialAssignMovingFrom_ = true;

    uint64_t memorySize = 0;
    uint64_t memoryAlignment = 1;

    uint64_t paddingOffset = 0;
    uint64_t paddingSize = 0;

    uint64_t elementMemorySize = 0;
    uint64_t elementMemoryAlignment = 1;
    uint64_t elementMemoryOffset = 0;

    uint64_t endPaddingOffset = 0;
    uint64_t endPaddingSize = 0;

    TypePtrList elementTypes;
    
    TypePtr dataTypeIndexType;
    uint64_t dataTypeIndexOffset = 0;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_VARIANT_LAYOUT_HPP