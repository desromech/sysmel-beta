#include "Environment/StructureType.hpp"
#include "Environment/Error.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/FieldVariable.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/AggregateTypeSequentialLayout.hpp"
#include <iostream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<StructureTypeValue> StructureTypeValueTypeRegistration;

bool StructureType::isStructureType() const
{
    return true;
}

AnyValuePtr StructureType::basicNewValue()
{
    auto sequentialLayout = getLayout().staticAs<AggregateTypeSequentialLayout> ();
    assert(sequentialLayout);

    const auto &slotTypes = sequentialLayout->getSlotTypes();

    auto result = basicMakeObject<StructureTypeValue> ();
    result->type = selfFromThis();
    result->slots.reserve(slotTypes.size());

    for(const auto &slotType : slotTypes)
        result->slots.push_back(validAnyValue(slotType->basicNewValue())->asMutableStoreValue());

    return result;
}

AggregateTypeLayoutPtr StructureType::makeLayoutInstance()
{
    return basicMakeObject<AggregateTypeSequentialLayout> ();
}

bool StructureTypeValue::isStructureTypeValue() const
{
    return true;
}

AnyValuePtr StructureTypeValue::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitStructureTypeValue(selfFromThis());
}

SExpression StructureTypeValue::asSExpression() const
{
    SExpressionList elementsSExpr;
    elementsSExpr.elements.reserve(slots.size());
    for(auto &el : slots)
        elementsSExpr.elements.push_back(el->asSExpression());

    return SExpressionList{{
        SExpressionIdentifier{{"struct"}},
        type->asSExpression(),
        elementsSExpr
    }};
}

TypePtr StructureTypeValue::getType() const
{
    return type;
}

AnyValuePtr StructureTypeValue::asMutableStoreValue()
{
    auto result = basicMakeObject<StructureTypeValue> ();
    result->type = type;
    result->slots.reserve(slots.size());
    for(auto &slot : slots)
        result->slots.push_back(slot->asMutableStoreValue());
    return result;
}

AnyValuePtr StructureTypeValue::getReferenceToSlotWithType(const int64_t slotIndex, const TypePtr &referenceType)
{
    assert(referenceType->isPointerLikeType());
    if(slotIndex < 0 || uint64_t(slotIndex) >= slots.size())
        signalNewWithMessage<Error> (formatString("Invalid slot index {0} for accessing structure of type {1}.", {castToString(slotIndex), type->printString()}));

    return referenceType.staticAs<PointerLikeType>()->makeWithValue(slots[slotIndex]);
}

} // End of namespace Environment
} // End of namespace Sysmel