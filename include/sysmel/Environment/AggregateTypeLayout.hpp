#ifndef SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_LAYOUT_HPP
#define SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_LAYOUT_HPP
#pragma once

#include "SimpleType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeLayout);


/**
 * I represent the layout of an aggregate type.
 */
class AggregateTypeLayout : public SubtypeOf<CompilerObject, AggregateTypeLayout>
{
public:
    static constexpr char const __typeName__[] = "AggregateTypeLayout";

    virtual uint64_t getMemorySize();
    virtual uint64_t getMemoryAlignment();

    virtual bool hasTrivialInitialization();
    virtual bool hasTrivialInitializationCopyingFrom();
    virtual bool hasTrivialInitializationMovingFrom();
    virtual bool hasTrivialFinalization();
    virtual bool hasTrivialCopyingFrom();
    virtual bool hasTrivialMovingFrom();

    /// Begin building a group.
    virtual void beginGroup();

    /// Adds a field variable onto the layout.
    virtual void addFieldVariable(const FieldVariablePtr &field);

    /// Add a slot with the specified type.
    virtual uint32_t addSlotWithType(const TypePtr &slotType);

    /// Finish building a group.
    virtual void finishGroup();

    /// Get the type for a specific slot and offset.
    virtual TypePtr getTypeForSlotAndOffset(int64_t slotIndex, int64_t slotOffset);
    
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AGGREGATE_TYPE_LAYOUT_HPP