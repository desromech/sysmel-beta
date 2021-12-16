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
class AggregateTypeSequentialLayout : public SubtypeOf<AggregateTypeLayout, AggregateTypeSequentialLayout>
{
public:
    static constexpr char const __typeName__[] = "AggregateTypeSequentialLayout";

    virtual uint64_t getMemorySize() override;
    virtual uint64_t getMemoryAlignment() override;

    virtual uint32_t addSlotWithType(const TypePtr &slotType) override;
    virtual void finishGroup() override;

    const TypePtrList &getSlotTypes() const
    {
        return slotTypes;
    }
    
protected:
    uint64_t memorySize = 0;
    uint64_t memoryAlignment = 1;
    TypePtrList slotTypes;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_SEQUENTIAL_LAYOUT_HPP