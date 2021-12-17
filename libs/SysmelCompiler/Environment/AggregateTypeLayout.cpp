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

bool AggregateTypeLayout::hasTrivialCopyingFrom()
{
    SysmelSelfSubclassResponsibility();
}

bool AggregateTypeLayout::hasTrivialMovingFrom()
{
    SysmelSelfSubclassResponsibility();
}

void AggregateTypeLayout::beginGroup()
{
}

void AggregateTypeLayout::addFieldVariable(const FieldVariablePtr &field)
{
    // TODO: Support bit fields.
    auto slotIndex = addSlotWithType(field->getValueType());
    field->setSlotIndex(slotIndex);
}

uint32_t AggregateTypeLayout::addSlotWithType(const TypePtr &)
{
    SysmelSelfSubclassResponsibility();
}

void AggregateTypeLayout::finishGroup()
{
}

} // End of namespace Environment
} // End of namespace Sysmel