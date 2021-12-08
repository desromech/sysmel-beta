#ifndef SYSMEL_ENVIRONMENT_DERIVED_TYPE_HPP
#define SYSMEL_ENVIRONMENT_DERIVED_TYPE_HPP
#pragma once

#include "SimpleType.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(DerivedType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(DerivedTypeValue);

/**
 * I am an instance of a function type object.
 */
class DerivedType : public SubMetaTypeOf<SimpleType, DerivedType>
{
public:
    virtual bool isDerivedType() const override;

    virtual const TypePtr &getBaseType();
    void setBaseType(const TypePtr &newBaseType);

protected:
    TypePtr baseType;
};

/**
 * I am an instance of a function type object.
 */
class DerivedTypeValue : public SubtypeOf<AnyValue, DerivedTypeValue>
{
public:
    static constexpr char const __typeName__[] = "DerivedType";
    static constexpr char const __sysmelTypeName__[] = "_DerivedType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isDerivedTypeValue() const override;

    DerivedTypePtr type;
    AnyValuePtr baseValue;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_DERIVED_TYPE_HPP