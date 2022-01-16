#include "Environment/AggregateTypeLayout.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/FieldVariable.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<AggregateTypeLayout> AggregateTypeLayoutTypeRegistration;

bool AggregateTypeLayout::supportsSequentialConstruction() const
{
    return false;
}

uint64_t AggregateTypeLayout::getSlotCount() const
{
    SysmelSelfSubclassResponsibility();
}

uint64_t AggregateTypeLayout::getMemorySize()
{
    SysmelSelfSubclassResponsibility();
}

uint64_t AggregateTypeLayout::getMemoryAlignment()
{
    SysmelSelfSubclassResponsibility();
}

bool AggregateTypeLayout::hasTrivialInitialization()
{
    SysmelSelfSubclassResponsibility();
}

bool AggregateTypeLayout::hasTrivialInitializationCopyingFrom()
{
    SysmelSelfSubclassResponsibility();
}

bool AggregateTypeLayout::hasTrivialInitializationMovingFrom()
{
    SysmelSelfSubclassResponsibility();
}

bool AggregateTypeLayout::hasTrivialFinalization()
{
    SysmelSelfSubclassResponsibility();
}

bool AggregateTypeLayout::hasTrivialAssignCopyingFrom()
{
    SysmelSelfSubclassResponsibility();
}

bool AggregateTypeLayout::hasTrivialAssignMovingFrom()
{
    SysmelSelfSubclassResponsibility();
}

void AggregateTypeLayout::beginGroup()
{
}

void AggregateTypeLayout::addVirtualMethods(const SpecificMethodPtrList &)
{
    SysmelSelfSubclassResponsibility();
}

void AggregateTypeLayout::addFieldVariable(const FieldVariablePtr &field)
{
    // TODO: Support bit fields.
    auto slotIndex = addSlotWithType(field->getValueType());
    field->setSlotIndex(slotIndex);
    field->setOffset(getOffsetForSlotIndex(slotIndex));
}

uint32_t AggregateTypeLayout::addSlotWithType(const TypePtr &)
{
    SysmelSelfSubclassResponsibility();
}

void AggregateTypeLayout::finishGroup()
{
}

TypePtr AggregateTypeLayout::getTypeForSlotAndOffset(int64_t slotIndex, int64_t slotOffset)
{
    (void)slotIndex;
    (void)slotOffset;
    SysmelSelfSubclassResponsibility();
}

TypePtr AggregateTypeLayout::getTypeForNonPaddingSlot(int64_t slotIndex)
{
    (void)slotIndex;
    SysmelSelfSubclassResponsibility();
}

uint64_t AggregateTypeLayout::getIndexForNonPaddingSlot(uint64_t slotIndex)
{
    (void)slotIndex;
    SysmelSelfSubclassResponsibility();
}

uint64_t AggregateTypeLayout::getOffsetForSlotIndex(uint32_t)
{
    return 0;
}

} // End of namespace Environment
} // End of namespace Sysmel