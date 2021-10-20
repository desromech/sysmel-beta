#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_UNION_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_UNION_TYPE_HPP
#pragma once

#include "AggregateTypeWithFields.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(UnionType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(UnionTypeValue);

/**
 * I am an instance of a function type object.
 */
class UnionType : public SubMetaTypeOf<AggregateTypeWithFields, UnionType>
{
public:
    virtual bool isUnionType() const override;
};

/**
 * I am an instance of a function type object.
 */
class UnionTypeValue : public SubtypeOf<AggregateTypeWithFieldsValue, UnionTypeValue>
{
public:
    static constexpr char const __typeName__[] = "UnionType";
    static constexpr char const __sysmelTypeName__[] = "_UnionType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isUnionTypeValue() const override;

    std::vector<uint8_t> data;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_UNION_TYPE_HPP