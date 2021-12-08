#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AGGREGATE_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AGGREGATE_TYPE_HPP
#pragma once

#include "SimpleType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateTypeValue);

/**
 * I am an instance of a function type object.
 */
class AggregateType : public SubMetaTypeOf<SimpleType, AggregateType>
{
public:
    virtual bool isAggregateType() const override;
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

    AggregateTypePtr type;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_FUNCTION_TYPE_HPP