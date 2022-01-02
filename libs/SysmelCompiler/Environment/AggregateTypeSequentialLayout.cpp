#include "Environment/AggregateTypeSequentialLayout.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/Utilities.hpp"
#include "Environment/VirtualTable.hpp"
#include "Environment/SpecificMethod.hpp"
#include "Environment/RuntimeContext.hpp"
#include <algorithm>
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<AggregateTypeSequentialLayout> AggregateTypeSequentialLayoutRegistration;

uint64_t AggregateTypeSequentialLayout::getMemorySize()
{
    return memorySize;
}

uint64_t AggregateTypeSequentialLayout::getMemoryAlignment()
{
    return memoryAlignment;
}

bool AggregateTypeSequentialLayout::hasTrivialInitialization()
{
    return hasTrivialInitialization_;
}

bool AggregateTypeSequentialLayout::hasTrivialInitializationCopyingFrom()
{
    return hasTrivialInitializationCopyingFrom_;
}

bool AggregateTypeSequentialLayout::hasTrivialInitializationMovingFrom()
{
    return hasTrivialInitializationMovingFrom_;
}

bool AggregateTypeSequentialLayout::hasTrivialFinalization()
{
    return hasTrivialFinalization_;
}

bool AggregateTypeSequentialLayout::hasTrivialAssignCopyingFrom()
{
    return hasTrivialAssignCopyingFrom_;
}

bool AggregateTypeSequentialLayout::hasTrivialAssignMovingFrom()
{
    return hasTrivialAssignMovingFrom_;
}

void AggregateTypeSequentialLayout::addInheritance(const AggregateTypeSequentialLayoutPtr &parentLayout)
{
    if(!parentLayout)
        return;

    // Inherit the triviality.
    hasTrivialInitialization_ = hasTrivialInitialization_ && parentLayout->hasTrivialInitialization();
    hasTrivialInitializationCopyingFrom_ = hasTrivialInitializationCopyingFrom_ && parentLayout->hasTrivialInitializationCopyingFrom();
    hasTrivialInitializationMovingFrom_ = hasTrivialInitializationMovingFrom_ && parentLayout->hasTrivialInitializationMovingFrom();
    hasTrivialFinalization_ = hasTrivialFinalization_ && parentLayout->hasTrivialFinalization();
    hasTrivialAssignCopyingFrom_ = hasTrivialAssignCopyingFrom_ && parentLayout->hasTrivialAssignCopyingFrom();
    hasTrivialAssignMovingFrom_ = hasTrivialAssignMovingFrom_ && parentLayout->hasTrivialAssignMovingFrom();

    // Append the parent layout slot types and offsets.
    const auto &parentSlotTypes = parentLayout->getSlotTypes();
    const auto &parentOffsets = parentLayout->getOffsets();

    slotTypes.insert(slotTypes.end(), parentSlotTypes.begin(), parentSlotTypes.end());
    offsets.reserve(parentOffsets.size());
    auto parentAlignment = parentLayout->getMemoryAlignment();
    if(memoryAlignment != 0 && parentAlignment)
    {
        memorySize = alignedTo(memorySize, parentAlignment);
        memoryAlignment = std::max(memoryAlignment, parentAlignment);
        auto extraOffset = memorySize;
        for(auto &parentSlotOffset : parentOffsets)
            offsets.push_back(parentSlotOffset + extraOffset);

        memorySize += parentLayout->getMemorySize();
    }
    else
    {
        offsets.insert(offsets.end(), parentOffsets.begin(), parentOffsets.end());
    }

    // Inherit the virtual table.
    if(parentLayout->virtualTable)
    {
        virtualTableSlotIndex = parentLayout->virtualTableSlotIndex;
        virtualTable = shallowCloneObject(parentLayout->virtualTable);
    }
}

void AggregateTypeSequentialLayout::addVirtualMethods(const SpecificMethodPtrList &virtualMethods)
{
    if(virtualMethods.empty())
        return;

    for(const auto &virtualMethod : virtualMethods)
        addVirtualMethod(virtualMethod);
}

void AggregateTypeSequentialLayout::ensureVirtualTableExists()
{
    if(virtualTable)
        return;

    virtualTable = basicMakeObject<VirtualTable> ();
    virtualTableSlotIndex = slotTypes.size();

    if(memoryAlignment != 0)
    {
        auto &targetDescription = RuntimeContext::getActive()->getTargetDescription();
        auto offset = alignedTo(memorySize, uint64_t(targetDescription.pointerAlignment));
        memoryAlignment = std::max(memoryAlignment, uint64_t(targetDescription.pointerAlignment));
        memorySize = offset + targetDescription.pointerSize;
        slotTypes.push_back(VirtualTable::__staticType__());
        offsets.push_back(offset);
    }
    else
    {
        slotTypes.push_back(VirtualTable::__staticType__());
        offsets.push_back(0);
    }
}

void AggregateTypeSequentialLayout::addVirtualMethod(const SpecificMethodPtr &virtualMethod)
{
    ensureVirtualTableExists();

    auto parentMethod = virtualMethod->getOverridenParentMethod();
    if(parentMethod)
    {
        sysmelAssert(virtualTableSlotIndex == parentMethod->getVirtualTableSlotIndex());
        
        auto entryIndex = parentMethod->getVirtualTableEntrySlotIndex();
        sysmelAssert(entryIndex < virtualTable->slots.size());
        virtualTable->slots[entryIndex] = virtualMethod;
        virtualMethod->setVirtualTableEntry(virtualTableSlotIndex, entryIndex);
    }
    else
    {
        virtualMethod->setVirtualTableEntry(virtualTableSlotIndex, virtualTable->slots.size());
        virtualTable->slots.push_back(virtualMethod);
    }
}

uint32_t AggregateTypeSequentialLayout::addSlotWithType(const TypePtr &slotType)
{
    hasTrivialInitialization_ = hasTrivialInitialization_ && slotType->hasTrivialInitialization();
    hasTrivialInitializationCopyingFrom_ = hasTrivialInitializationCopyingFrom_ && slotType->hasTrivialInitializationCopyingFrom();
    hasTrivialInitializationMovingFrom_ = hasTrivialInitializationMovingFrom_ && slotType->hasTrivialInitializationMovingFrom();
    hasTrivialFinalization_ = hasTrivialFinalization_ && slotType->hasTrivialFinalization();
    hasTrivialAssignCopyingFrom_ = hasTrivialAssignCopyingFrom_ && slotType->hasTrivialAssignCopyingFrom();
    hasTrivialAssignMovingFrom_ = hasTrivialAssignMovingFrom_ && slotType->hasTrivialAssignMovingFrom();

    auto offset = memorySize;
    if(memoryAlignment != 0)
    {
        auto slotSize = slotType->getValueSize();
        auto slotAlignment = slotType->getValueAlignment();
        if(slotAlignment == 0)
        {
            memoryAlignment = 0;
            memorySize = 0;
            offset = 0;
        }
        else
        {
            if(isPacked_)
            {
                offset = memorySize;
                memorySize = offset + slotSize;
            }
            else
            {
                offset = alignedTo(memorySize, slotAlignment);
                memorySize = offset + slotSize;
                memoryAlignment = std::max(memoryAlignment, slotAlignment);
            }
        }
    }

    auto slotIndex = uint32_t(slotTypes.size());
    slotTypes.push_back(slotType);
    offsets.push_back(offset);
    return slotIndex;
}

void AggregateTypeSequentialLayout::finishGroup()
{
    if(memoryAlignment != 0)
        memorySize = alignedTo(memorySize, memoryAlignment);
}

TypePtr AggregateTypeSequentialLayout::getTypeForSlotAndOffset(int64_t slotIndex, int64_t slotOffset)
{
    (void)slotOffset;
    if(slotIndex < 0 || size_t(slotIndex) >= slotTypes.size())
        return nullptr;

    return slotTypes[slotIndex];
}

} // End of namespace Environment
} // End of namespace Sysmel