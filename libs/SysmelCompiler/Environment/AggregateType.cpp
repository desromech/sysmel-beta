#include "Environment/AggregateType.hpp"
#include "Environment/AggregateTypeLayout.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/AggregateElementReference.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<AggregateTypeValue> AggregateTypeValueTypeRegistration;

bool AggregateType::isAggregateType() const
{
    return true;
}

uint64_t AggregateType::getMemorySize()
{
    getLayout();
    return layout ? layout->getMemorySize() : 0;
}

uint64_t AggregateType::getMemoryAlignment()
{
    getLayout();
    return layout ? layout->getMemoryAlignment() : 0;
}

bool AggregateType::isImmutableType()
{
    return false;
}

bool AggregateType::isPassedByReference()
{
    return true;
}

bool AggregateType::isReturnedByReference()
{
    return true;
}

TypePtr AggregateType::asConstReceiverType()
{
    return selfFromThis()->withConst()->ref();
}

TypePtr AggregateType::asReceiverType()
{
    return selfFromThis()->ref();
}

bool AggregateType::hasTrivialInitialization()
{
    return SuperType::hasTrivialInitialization() && getLayout() && layout->hasTrivialInitialization();
}

bool AggregateType::hasTrivialInitializationCopyingFrom()
{
    return SuperType::hasTrivialInitializationCopyingFrom() && getLayout() && layout->hasTrivialInitializationCopyingFrom();
}

bool AggregateType::hasTrivialInitializationMovingFrom()
{
    return SuperType::hasTrivialInitializationMovingFrom() && getLayout() && layout->hasTrivialInitializationMovingFrom();
}

bool AggregateType::hasTrivialFinalization()
{
    return SuperType::hasTrivialFinalization() && getLayout() && layout->hasTrivialFinalization();
}

bool AggregateType::hasTrivialCopyingFrom()
{
    return SuperType::hasTrivialCopyingFrom() && getLayout() && layout->hasTrivialCopyingFrom();
}

bool AggregateType::hasTrivialMovingFrom()
{
    return SuperType::hasTrivialMovingFrom() && getLayout() && layout->hasTrivialMovingFrom();
}

const AggregateTypeLayoutPtr &AggregateType::getLayout()
{
    if(!layout)
        buildLayout();

    return layout;
}

void AggregateType::buildLayout()
{
    SysmelSelfSubclassResponsibility();
}

bool AggregateTypeValue::isAggregateTypeValue() const
{
    return true;
}

AnyValuePtr AggregateTypeValue::asMutableStoreValue()
{
    return shallowClone();
}

AnyValuePtr AggregateTypeValue::getReferenceToSlotWithType(int64_t slotIndex, int64_t slotOffset, const TypePtr &referenceType)
{
    assert(referenceType->isPointerLikeType());
    auto layout = type->getLayout();
    auto elementType = layout->getTypeForSlotAndOffset(slotIndex, slotOffset);
    if(!elementType)
        signalNewWithMessage<Error> (formatString("Invalid slot index {0} and/or offset: {1} for accessing aggregate of type {1}.", {castToString(slotIndex), castToString(slotOffset), type->printString()}));

    auto referenceValue = AggregateElementReference::make(selfFromThis(), elementType, slotIndex, slotOffset);
    return referenceType.staticAs<PointerLikeType>()->makeWithValue(referenceValue);
}

AnyValuePtr AggregateTypeValue::loadAggregateElement(int64_t slotIndex, int64_t slotOffset, const TypePtr &elementType)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr AggregateTypeValue::copyAssignAggregateElement(int64_t slotIndex, int64_t slotOffset, const TypePtr &elementType, const AnyValuePtr &newValue)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr AggregateTypeValue::moveAssignAggregateElement(int64_t slotIndex, int64_t slotOffset, const TypePtr &elementType, const AnyValuePtr &newValue)
{
    SysmelSelfSubclassResponsibility();
}

} // End of namespace Environment
} // End of namespace Sysmel
