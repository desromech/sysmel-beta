#ifndef SYSMEL_ENVIRONMENT_STRUCTURE_TYPE_HPP
#define SYSMEL_ENVIRONMENT_STRUCTURE_TYPE_HPP
#pragma once

#include "AggregateTypeWithFields.hpp"

namespace Sysmel
{
namespace Environment
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
    
    virtual AnyValuePtr basicNewValue() override;
    
protected:
    virtual AggregateTypeLayoutPtr makeLayoutInstance() override;
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
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
    virtual TypePtr getType() const override;
    virtual AnyValuePtr asMutableStoreValue() override;
    virtual AnyValuePtr getReferenceToSlotWithType(const int64_t slotIndex, const TypePtr &referenceType) override;

    AnyValuePtrList slots;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_STRUCTURE_TYPE_HPP