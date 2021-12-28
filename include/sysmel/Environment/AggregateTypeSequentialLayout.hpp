#ifndef SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_SEQUENTIAL_LAYOUT_HPP
#define SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_SEQUENTIAL_LAYOUT_HPP
#pragma once

#include "AggregateTypeLayout.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeSequentialLayout);


/**
 * I represent the layout of an aggregate type whose elements are laid on sequentially.
 */
class SYSMEL_COMPILER_LIB_EXPORT AggregateTypeSequentialLayout : public SubtypeOf<AggregateTypeLayout, AggregateTypeSequentialLayout>
{
public:
    static constexpr char const __typeName__[] = "AggregateTypeSequentialLayout";

    virtual uint64_t getMemorySize() override;
    virtual uint64_t getMemoryAlignment() override;

    virtual bool hasTrivialInitialization() override;
    virtual bool hasTrivialInitializationCopyingFrom() override;
    virtual bool hasTrivialInitializationMovingFrom() override;
    virtual bool hasTrivialFinalization() override;
    virtual bool hasTrivialAssignCopyingFrom() override;
    virtual bool hasTrivialAssignMovingFrom() override;

    virtual uint32_t addSlotWithType(const TypePtr &slotType) override;
    virtual void finishGroup() override;

    virtual TypePtr getTypeForSlotAndOffset(int64_t slotIndex, int64_t slotOffset) override;

    const TypePtrList &getSlotTypes() const
    {
        return slotTypes;
    }
    
    std::vector<uint64_t> getOffsets() const
    {
        return offsets;
    }

protected:
    uint64_t memorySize = 0;
    uint64_t memoryAlignment = 1;

    bool hasTrivialInitialization_ = true;
    bool hasTrivialInitializationCopyingFrom_ = true;
    bool hasTrivialInitializationMovingFrom_ = true;
    bool hasTrivialFinalization_ = true;
    bool hasTrivialAssignCopyingFrom_ = true;
    bool hasTrivialAssignMovingFrom_ = true;

    TypePtrList slotTypes;
    std::vector<uint64_t> offsets;

};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_SEQUENTIAL_LAYOUT_HPP