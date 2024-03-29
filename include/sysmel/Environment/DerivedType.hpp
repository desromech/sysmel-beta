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
class SYSMEL_COMPILER_LIB_EXPORT DerivedType : public SubMetaTypeOf<SimpleType, DerivedType>
{
public:
    virtual bool isDerivedType() const override;

    virtual TypePtr getBaseType() override;
    void setBaseType(const TypePtr &newBaseType);

protected:
    TypePtr baseType;
};

/**
 * I am an instance of a function type object.
 */
class SYSMEL_COMPILER_LIB_EXPORT DerivedTypeValue : public SubtypeOf<AnyValue, DerivedTypeValue>
{
public:
    static constexpr char const __typeName__[] = "DerivedType";
    static constexpr char const __sysmelTypeName__[] = "_DerivedType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    static MethodCategories __typeMethods__();

    virtual bool isDerivedTypeValue() const override;

    DerivedTypePtr type;
    AnyValuePtr baseValue;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_DERIVED_TYPE_HPP