#include "Environment/AggregateElementReference.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<AggregateElementReference> valueBoxTypeRegistration;

AggregateElementReferencePtr AggregateElementReference::make(const AnyValuePtr &aggregate, const TypePtr &elementType, int64_t slotIndex, int64_t slotOffset)
{
    auto result = basicMakeObject<AggregateElementReference> ();
    result->aggregate = aggregate;
    result->type = elementType;
    result->slotIndex = slotIndex;
    result->slotOffset = slotOffset;
    return result;
}

bool AggregateElementReference::isAggregateElementReference() const
{
    return true;
}

AnyValuePtr AggregateElementReference::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitAggregateElementReference(selfFromThis());
}

SExpression AggregateElementReference::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"aggregateElementReference"}},
        type->asSExpression(),
        aggregate ? aggregate->asSExpression() : nullptr,
        LargeInteger{slotIndex},
        LargeInteger{slotOffset},
    }};
}

AnyValuePtr AggregateElementReference::copyAssignValue(const AnyValuePtr &newValue)
{
    validAnyValue(aggregate)->copyAssignAggregateElement(slotIndex, slotOffset, type, newValue);
    return selfFromThis();
}

AnyValuePtr AggregateElementReference::moveAssignValue(const AnyValuePtr &newValue)
{
    validAnyValue(aggregate)->moveAssignAggregateElement(slotIndex, slotOffset, type, newValue);
    return selfFromThis();
}

AnyValuePtr AggregateElementReference::accessVariableAsReferenceWithType(const TypePtr &referenceType)
{
    if(referenceType->isReferenceLikeType())
        return referenceType.staticAs<PointerLikeType> ()->makeWithValue(selfFromThis());
    return selfFromThis();
}

AnyValuePtr AggregateElementReference::accessVariableAsValueWithType(const TypePtr &valueType)
{
    return validAnyValue(aggregate)->loadAggregateElement(slotIndex, slotOffset, valueType);
}

AnyValuePtr AggregateElementReference::asMutableStoreValue()
{
    return accessVariableAsValueWithType(type)->asMutableStoreValue();
}

} // End of namespace Environment
} // End of namespace Sysmel
