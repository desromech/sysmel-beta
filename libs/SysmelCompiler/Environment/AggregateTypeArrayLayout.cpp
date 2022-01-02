#include "Environment/AggregateTypeArrayLayout.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<AggregateTypeArrayLayout> AggregateTypeArrayLayoutTypeRegistration;

uint64_t AggregateTypeArrayLayout::getMemorySize()
{
    return memorySize;
}

uint64_t AggregateTypeArrayLayout::getMemoryAlignment()
{
    return memoryAlignment;
}

bool AggregateTypeArrayLayout::hasTrivialInitialization()
{
    return size == 0 || elementType->hasTrivialInitialization();
}

bool AggregateTypeArrayLayout::hasTrivialInitializationCopyingFrom()
{
    return size == 0 || elementType->hasTrivialInitializationCopyingFrom();
}

bool AggregateTypeArrayLayout::hasTrivialInitializationMovingFrom()
{
    return size == 0 || elementType->hasTrivialInitializationMovingFrom();
}

bool AggregateTypeArrayLayout::hasTrivialFinalization()
{
    return size == 0 || elementType->hasTrivialFinalization();
}

bool AggregateTypeArrayLayout::hasTrivialAssignCopyingFrom()
{
    return size == 0 || elementType->hasTrivialAssignCopyingFrom();
}

bool AggregateTypeArrayLayout::hasTrivialAssignMovingFrom()
{
    return size == 0 || elementType->hasTrivialAssignMovingFrom();
}

TypePtr AggregateTypeArrayLayout::getTypeForSlotAndOffset(int64_t, int64_t)
{
    return elementType;
}

TypePtr AggregateTypeArrayLayout::getTypeForNonPaddingSlot(int64_t slotIndex)
{
    return elementType;
}

uint64_t AggregateTypeArrayLayout::getIndexForNonPaddingSlot(uint64_t slotIndex)
{
    return slotIndex;
}

void AggregateTypeArrayLayout::setElementTypeAndSize(const TypePtr &newElementType, uint64_t newSize)
{
    elementType = newElementType;
    size = newSize;

    memorySize = size*elementType->getAlignedMemorySize();
    memoryAlignment = elementType->getMemoryAlignment();
}

} // End of namespace Environment
} // End of namespace Sysmel