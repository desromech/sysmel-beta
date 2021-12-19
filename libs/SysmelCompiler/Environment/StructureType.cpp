#include "Environment/StructureType.hpp"
#include "Environment/Error.hpp"
#include "Environment/FieldVariable.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/LiteralValueVisitor.hpp"
#include "Environment/TypeVisitor.hpp"
#include "Environment/Error.hpp"
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
        result->slots.push_back(slotType->basicNewValue());

    return result;
}

AnyValuePtr StructureType::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitStructureType(selfFromThis());
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

AnyValuePtr StructureTypeValue::loadAggregateElement(int64_t slotIndex, int64_t slotOffset, const TypePtr &elementType)
{
    (void)slotOffset;
    if(slotIndex < 0 || size_t(slotIndex) >= slots.size())
        signalNewWithMessage<Error> ("Invalid aggregate load element.");

    return validAnyValue(slots[slotIndex])->accessVariableAsValueWithType(elementType);
}

AnyValuePtr StructureTypeValue::copyAssignAggregateElement(int64_t slotIndex, int64_t slotOffset, const TypePtr &elementType, const AnyValuePtr &newValue)
{
    (void)slotOffset;
    (void)elementType;
    if(slotIndex < 0 || size_t(slotIndex) >= slots.size())
        signalNewWithMessage<Error> ("Invalid aggregate move asign element.");

    slots[slotIndex] = newValue;
    return newValue;
}

AnyValuePtr StructureTypeValue::moveAssignAggregateElement(int64_t slotIndex, int64_t slotOffset, const TypePtr &elementType, const AnyValuePtr &newValue)
{
    (void)slotOffset;
    (void)elementType;
    if(slotIndex < 0 || size_t(slotIndex) >= slots.size())
        signalNewWithMessage<Error> ("Invalid aggregate move asign element.");

    slots[slotIndex] = newValue;
    return newValue;
}

} // End of namespace Environment
} // End of namespace Sysmel