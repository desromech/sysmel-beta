#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_STRUCTURE_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_STRUCTURE_TYPE_HPP
#pragma once

#include "AggregateTypeWithFields.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(StructureType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(StructureTypeValue);

/**
 * I am an instance of a function type object.
 */
class StructureType : public SubMetaTypeOf<AggregateTypeWithFields, StructureType>
{
public:
    virtual bool isStructureType() const override;
};

/**
 * I am an instance of a function type object.
 */
class StructureTypeValue : public SubtypeOf<AggregateTypeWithFieldsValue, StructureTypeValue>
{
public:
    static constexpr char const __typeName__[] = "StructureType";
    static constexpr char const __sysmelTypeName__[] = "_StructureType";

    static constexpr bool __isDynamicCompileTimeType__ = false;

    virtual bool isStructureTypeValue() const override;

    AnyValuePtrList slots;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_STRUCTURE_TYPE_HPP