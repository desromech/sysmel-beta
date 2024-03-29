#include "Environment/AggregateTypeVariantLayout.hpp"
#include "Environment/PaddingType.hpp"
#include "Environment/PrimitiveIntegerType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/Utilities.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<AggregateTypeVariantLayout> AggregateTypeVariantLayoutTypeRegistration;

uint64_t AggregateTypeVariantLayout::getSlotCount() const
{
    return 2 + (paddingSize > 0 ? 1 : 0) + (endPaddingSize > 0 ? 1 : 0);
}

void AggregateTypeVariantLayout::beginGroup()
{
}

uint64_t AggregateTypeVariantLayout::getMemorySize()
{
    return memorySize;
}

uint64_t AggregateTypeVariantLayout::getMemoryAlignment()
{
    return memoryAlignment;
}

bool AggregateTypeVariantLayout::hasTrivialInitialization()
{
    return hasTrivialInitialization_;
}

bool AggregateTypeVariantLayout::hasTrivialInitializationCopyingFrom()
{
    return hasTrivialInitializationCopyingFrom_;
}

bool AggregateTypeVariantLayout::hasTrivialInitializationMovingFrom()
{
    return hasTrivialInitializationMovingFrom_;
}

bool AggregateTypeVariantLayout::hasTrivialFinalization()
{
    return hasTrivialFinalization_;
}

bool AggregateTypeVariantLayout::hasTrivialAssignCopyingFrom()
{
    return hasTrivialAssignCopyingFrom_;
}

bool AggregateTypeVariantLayout::hasTrivialAssignMovingFrom()
{
    return hasTrivialAssignMovingFrom_;
}

uint32_t AggregateTypeVariantLayout::addSlotWithType(const TypePtr &slotType)
{
    hasTrivialInitialization_ = hasTrivialInitialization_ && slotType->hasTrivialInitialization();
    hasTrivialInitializationCopyingFrom_ = hasTrivialInitializationCopyingFrom_ && slotType->hasTrivialInitializationCopyingFrom();
    hasTrivialInitializationMovingFrom_ = hasTrivialInitializationMovingFrom_ && slotType->hasTrivialInitializationMovingFrom();
    hasTrivialFinalization_ = hasTrivialFinalization_ && slotType->hasTrivialFinalization();
    hasTrivialAssignCopyingFrom_ = hasTrivialAssignCopyingFrom_ && slotType->hasTrivialAssignCopyingFrom();
    hasTrivialAssignMovingFrom_ = hasTrivialAssignMovingFrom_ && slotType->hasTrivialAssignMovingFrom();

    if(elementMemoryAlignment != 0)
    {
        auto slotSize = slotType->getValueSize();
        auto slotAlignment = slotType->getValueAlignment();
        if(slotAlignment == 0)
        {
            elementMemoryAlignment = 0;
            elementMemorySize = 0;
        }
        else
        {
            elementMemorySize = std::max(elementMemorySize, slotSize);
            elementMemoryAlignment = std::max(elementMemoryAlignment, slotAlignment);
        }
    }

    return 0;
}

void AggregateTypeVariantLayout::finishGroup()
{
    if(elementMemoryAlignment == 0)
        return;

    sysmelAssert(elementTypes.size() <= 0xFF);
    dataTypeIndexOffset = 0;
    dataTypeIndexType = UInt8::__staticType__();

    paddingOffset = dataTypeIndexType->getMemorySize();
    elementMemoryOffset = alignedTo(paddingOffset, elementMemoryAlignment);
    paddingSize = elementMemoryOffset - paddingOffset;

    elementMemorySize = alignedTo(elementMemorySize, elementMemoryAlignment);

    memoryAlignment = std::max(dataTypeIndexType->getMemoryAlignment(), elementMemoryAlignment);
    endPaddingOffset = elementMemoryOffset + elementMemorySize;
    memorySize = alignedTo(endPaddingOffset, memoryAlignment);
    endPaddingSize = memorySize - endPaddingOffset;
}

TypePtr AggregateTypeVariantLayout::getTypeForSlotAndOffset(int64_t slotIndex, int64_t slotOffset)
{
    (void)slotOffset;
    switch(slotIndex)
    {
    case 0: return dataTypeIndexType;
    case 1: return paddingSize > 0 ? PaddingType::__staticType__() : Type::getVoidType();
    case 2: return paddingSize > 0 ? Type::getVoidType() : TypePtr();
    case 3: return paddingSize > 0 && endPaddingSize > 0 ? PaddingType::__staticType__() : TypePtr();
    default:
        return nullptr;
    }
}

TypePtr AggregateTypeVariantLayout::getTypeForNonPaddingSlot(int64_t slotIndex)
{
    return getTypeForSlotAndOffset(slotIndex, 0);
}

uint64_t AggregateTypeVariantLayout::getOffsetForSlotIndex(uint32_t slotIndex)
{    switch(slotIndex)
    {
    case 0: return 0;
    case 1: return paddingSize > 0 ? paddingOffset : elementMemoryOffset;
    case 2: return paddingSize > 0 ? elementMemoryOffset : endPaddingOffset;
    case 3: return paddingSize > 0 && endPaddingSize > 0 ? endPaddingOffset : 0;
    default:
        return 0;
    }
}

} // End of namespace Environment
} // End of namespace Sysmel