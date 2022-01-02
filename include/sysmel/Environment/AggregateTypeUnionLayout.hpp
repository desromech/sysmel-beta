#ifndef SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_UNION_LAYOUT_HPP
#define SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_UNION_LAYOUT_HPP
#pragma once

#include "AggregateTypeLayout.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeUnionLayout);


/**
 * I represent the layout of an aggregate type whose elements all share the same position.
 */
class SYSMEL_COMPILER_LIB_EXPORT AggregateTypeUnionLayout : public SubtypeOf<AggregateTypeLayout, AggregateTypeUnionLayout>
{
public:
    static constexpr char const __typeName__[] = "AggregateTypeUnionLayout";

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

protected:
    uint64_t memorySize = 0;
    uint64_t memoryAlignment = 1;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_UNION_LAYOUT_HPP