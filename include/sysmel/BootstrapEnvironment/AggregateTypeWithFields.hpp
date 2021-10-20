#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AGGREGATE_TYPE_WITH_FIELDS_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AGGREGATE_TYPE_WITH_FIELDS_HPP
#pragma once

#include "AggregateType.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeWithFields);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeWithFieldsValue);

/**
 * I am an instance of a function type object.
 */
class AggregateTypeWithFields : public SubMetaTypeOf<AggregateType, AggregateTypeWithFields>
{
public:
    virtual bool isAggregateTypeWithFields() const override;
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
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AGGREGATE_TYPE_WITH_FIELDS_HPP