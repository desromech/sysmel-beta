#include "Environment/AggregateTypeUnionLayout.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/Utilities.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<AggregateTypeUnionLayout> AggregateTypeUnionLayoutTypeRegistration;

uint64_t AggregateTypeUnionLayout::getMemorySize()
{
    return memorySize;
}

uint64_t AggregateTypeUnionLayout::getMemoryAlignment()
{
    return memoryAlignment;
}

bool AggregateTypeUnionLayout::hasTrivialInitialization()
{
    return true;
}

bool AggregateTypeUnionLayout::hasTrivialInitializationCopyingFrom()
{
    return true;
}

bool AggregateTypeUnionLayout::hasTrivialInitializationMovingFrom()
{
    return true;
}

bool AggregateTypeUnionLayout::hasTrivialFinalization()
{
    return true;
}

bool AggregateTypeUnionLayout::hasTrivialAssignCopyingFrom()
{
    return true;
}

bool AggregateTypeUnionLayout::hasTrivialAssignMovingFrom()
{
    return true;
}

void AggregateTypeUnionLayout::beginGroup()
{
}

uint32_t AggregateTypeUnionLayout::addSlotWithType(const TypePtr &slotType)
{
    memorySize = std::max(memorySize, slotType->getMemorySize());
    memoryAlignment = std::max(memorySize, slotType->getMemoryAlignment());
    return 0;
}

void AggregateTypeUnionLayout::finishGroup()
{
    memorySize = alignedTo(memorySize, memoryAlignment);
}

} // End of namespace Environment
} // End of namespace Sysmel