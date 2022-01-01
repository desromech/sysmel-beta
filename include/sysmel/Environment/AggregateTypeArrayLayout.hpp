#ifndef SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_ARRAY_LAYOUT_HPP
#define SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_ARRAY_LAYOUT_HPP
#pragma once

#include "AggregateTypeLayout.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeArrayLayout);


/**
 * I represent the layout of an array aggregate type.
 */
class SYSMEL_COMPILER_LIB_EXPORT AggregateTypeArrayLayout : public SubtypeOf<AggregateTypeLayout, AggregateTypeLayout>
{
public:
    static constexpr char const __typeName__[] = "AggregateTypeArrayLayout";

    virtual uint64_t getMemorySize() override;
    virtual uint64_t getMemoryAlignment() override;

    virtual bool hasTrivialInitialization() override;
    virtual bool hasTrivialInitializationCopyingFrom() override;
    virtual bool hasTrivialInitializationMovingFrom() override;
    virtual bool hasTrivialFinalization() override;
    virtual bool hasTrivialAssignCopyingFrom() override;
    virtual bool hasTrivialAssignMovingFrom() override;

    virtual TypePtr getTypeForSlotAndOffset(int64_t slotIndex, int64_t slotOffset) override;

    void setElementTypeAndSize(const TypePtr &newElementType, uint64_t newSize);

protected:
    TypePtr elementType;
    uint64_t size = 0;
    uint64_t memorySize = 0;
    uint64_t memoryAlignment = 0;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_ARRAY_LAYOUT_HPP