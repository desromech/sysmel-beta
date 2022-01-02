#ifndef SYSMEL_ENVIRONMENT_UNION_TYPE_HPP
#define SYSMEL_ENVIRONMENT_UNION_TYPE_HPP
#pragma once

#include "AggregateTypeWithFields.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(UnionType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(UnionTypeValue);

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT UnionType : public SubMetaTypeOf<AggregateTypeWithFields, UnionType>
{
public:
    virtual bool isUnionType() const override;
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor) override;

protected:
    virtual AggregateTypeLayoutPtr makeLayoutInstance() override;
};

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT UnionTypeValue : public SubtypeOf<AggregateTypeWithFieldsValue, UnionTypeValue>
{
public:
    static constexpr char const __typeName__[] = "UnionType";
    static constexpr char const __sysmelTypeName__[] = "_UnionType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isUnionTypeValue() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;

    std::vector<uint8_t> data;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_UNION_TYPE_HPP