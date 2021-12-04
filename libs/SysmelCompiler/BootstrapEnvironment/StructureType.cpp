#include "sysmel/BootstrapEnvironment/StructureType.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"
#include "sysmel/BootstrapEnvironment/PointerLikeType.hpp"
#include "sysmel/BootstrapEnvironment/FieldVariable.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"
#include <iostream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<StructureTypeValue> StructureTypeValueTypeRegistration;

bool StructureType::isStructureType() const
{
    return true;
}

AnyValuePtr StructureType::basicNewValue()
{
    auto result = basicMakeObject<StructureTypeValue> ();
    result->type = selfFromThis();
    result->slots.reserve(fields.size());

    // TODO: Use a proper structure layout here!!!.
    for(auto &field : fields)
        result->slots.push_back(validAnyValue(field->getValueType()->basicNewValue())->asMutableStoreValue());
    // TODO: Initialize the slots.
    return result;
}

bool StructureTypeValue::isStructureTypeValue() const
{
    return true;
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius