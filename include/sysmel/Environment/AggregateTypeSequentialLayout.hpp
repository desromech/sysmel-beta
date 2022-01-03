#ifndef SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_SEQUENTIAL_LAYOUT_HPP
#define SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_SEQUENTIAL_LAYOUT_HPP
#pragma once

#include "AggregateTypeLayout.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeSequentialLayout);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(VirtualTable);


/**
 * I represent the layout of an aggregate type whose elements are laid on sequentially.
 */
class SYSMEL_COMPILER_LIB_EXPORT AggregateTypeSequentialLayout : public SubtypeOf<AggregateTypeLayout, AggregateTypeSequentialLayout>
{
public:
    static constexpr char const __typeName__[] = "AggregateTypeSequentialLayout";

    virtual bool supportsSequentialConstruction() const override;
    virtual uint64_t getSlotCount() const override;

    virtual uint64_t getMemorySize() override;
    virtual uint64_t getMemoryAlignment() override;

    virtual bool hasTrivialInitialization() override;
    virtual bool hasTrivialInitializationCopyingFrom() override;
    virtual bool hasTrivialInitializationMovingFrom() override;
    virtual bool hasTrivialFinalization() override;
    virtual bool hasTrivialAssignCopyingFrom() override;
    virtual bool hasTrivialAssignMovingFrom() override;

    virtual void addInheritance(const AggregateTypeSequentialLayoutPtr &parentLayout);
    virtual void addVirtualMethods(const SpecificMethodPtrList &virtualMethods) override;
    virtual uint32_t addSlotWithType(const TypePtr &slotType) override;
    virtual void finishGroup() override;

    virtual TypePtr getTypeForSlotAndOffset(int64_t slotIndex, int64_t slotOffset) override;
    virtual TypePtr getTypeForNonPaddingSlot(int64_t slotIndex) override;
    virtual uint64_t getIndexForNonPaddingSlot(uint64_t slotIndex) override;

    const TypePtrList &getSlotTypes() const
    {
        return slotTypes;
    }
    
    std::vector<uint64_t> getOffsets() const
    {
        return offsets;
    }

    bool isPacked() const
    {
        return isPacked_;
    }

    void setPacked(bool newPacked)
    {
        isPacked_ = newPacked;
    }

    const VirtualTablePtr &getVirtualTable() const
    {
        return virtualTable;
    }

    uint32_t getVirtualTableSlotIndex() const
    {
        return virtualTableSlotIndex;
    }

    const std::vector<size_t> &getNonPaddingSlotIndices() const
    {
        return nonPaddingSlotIndices;
    }

protected:
    void ensureVirtualTableExists();
    uint64_t nextOffsetAlignedTo(uint64_t alignment);
    void addPadding(uint64_t size);
    void addVirtualMethod(const SpecificMethodPtr &virtualMethod);

    uint64_t memorySize = 0;
    uint64_t memoryAlignment = 1;

    bool hasTrivialInitialization_ = true;
    bool hasTrivialInitializationCopyingFrom_ = true;
    bool hasTrivialInitializationMovingFrom_ = true;
    bool hasTrivialFinalization_ = true;
    bool hasTrivialAssignCopyingFrom_ = true;
    bool hasTrivialAssignMovingFrom_ = true;

    bool isPacked_ = false;

    VirtualTablePtr virtualTable;
    uint32_t virtualTableSlotIndex = 0;

    TypePtrList slotTypes;
    std::vector<uint64_t> offsets;
    std::vector<size_t> nonPaddingSlotIndices;

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_SEQUENTIAL_LAYOUT_HPP