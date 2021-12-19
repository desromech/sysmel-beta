#ifndef SYSMEL_ENVIRONMENT_AGGREGATE_ELEMENT_REFERENCE_HPP
#define SYSMEL_ENVIRONMENT_AGGREGATE_ELEMENT_REFERENCE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(AggregateElementReference);

/**
 * I am a value that describes a reference onto a specific aggregate type element.
 */
class AggregateElementReference : public SubtypeOf<CompilerObject, AggregateElementReference>
{
public:
    static constexpr char const __typeName__[] = "AggregateElementReference";

    static AggregateElementReferencePtr make(const AnyValuePtr &aggregate, const TypePtr &elementType, int64_t slotIndex, int64_t slotOffset);

    virtual bool isAggregateElementReference() const override;
    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;
    virtual SExpression asSExpression() const override;
    virtual AnyValuePtr copyAssignValue(const AnyValuePtr &newValue) override;
    virtual AnyValuePtr moveAssignValue(const AnyValuePtr &newValue) override;
    
    virtual AnyValuePtr accessVariableAsReferenceWithType(const TypePtr &referenceType) override;
    virtual AnyValuePtr accessVariableAsValueWithType(const TypePtr &valueType) override;

    virtual AnyValuePtr asMutableStoreValue() override;

    AnyValuePtr aggregate;
    TypePtr type;
    int64_t slotIndex = 0;
    int64_t slotOffset = 0;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AGGREGATE_ELEMENT_REFERENCE_HPP
