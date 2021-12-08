#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AGGREGATE_TYPE_WITH_FIELDS_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AGGREGATE_TYPE_WITH_FIELDS_HPP
#pragma once

#include "AggregateType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeWithFields);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeWithFieldsValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(FieldVariable);

/**
 * I am an instance of a function type object.
 */
class AggregateTypeWithFields : public SubMetaTypeOf<AggregateType, AggregateTypeWithFields>
{
public:
    virtual bool isAggregateTypeWithFields() const override;

    virtual bool canHaveUserDefinedFields() const override;
    
};

/**
 * I am an instance of a function type object.
 */
class AggregateTypeWithFieldsValue : public SubtypeOf<AggregateTypeValue, AggregateTypeWithFieldsValue>
{
public:
    static constexpr char const __typeName__[] = "AggregateTypeWithFields";
    static constexpr char const __sysmelTypeName__[] = "_AggregateTypeWithFields";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isAggregateTypeWithFieldsValue() const override;

    virtual AnyValuePtr getReferenceToFieldWithType(const FieldVariablePtr &field, const TypePtr &referenceType) override;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AGGREGATE_TYPE_WITH_FIELDS_HPP