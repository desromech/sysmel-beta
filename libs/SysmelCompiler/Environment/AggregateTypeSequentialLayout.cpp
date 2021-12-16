#include "Environment/AggregateTypeSequentialLayout.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/Utilities.hpp"
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

uint32_t AggregateTypeSequentialLayout::addSlotWithType(const TypePtr &slotType)
{
    if(memoryAlignment != 0)
    {
        auto slotSize = slotType->getValueSize();
        auto slotAlignment = slotType->getValueAlignment();
        if(slotAlignment == 0)
        {
            memoryAlignment = 0;
            memorySize = 0;
        }
        else
        {
            memorySize = alignedTo(memorySize, slotAlignment) + slotSize;
            memoryAlignment = std::max(memoryAlignment, slotAlignment);
        }
    }

    auto slotIndex = uint32_t(slotTypes.size());
    slotTypes.push_back(slotType);
    return slotIndex;
}

void AggregateTypeSequentialLayout::finishGroup()
{
    if(memoryAlignment != 0)
        memorySize = alignedTo(memorySize, memoryAlignment);
}

} // End of namespace Environment
} // End of namespace Sysmel