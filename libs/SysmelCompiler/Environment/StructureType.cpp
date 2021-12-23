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
    sysmelAssert(sequentialLayout);

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

} // End of namespace Environment
} // End of namespace Sysmel