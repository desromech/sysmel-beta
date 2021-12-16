#include "Environment/AggregateType.hpp"
#include "Environment/AggregateTypeLayout.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
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

} // End of namespace Environment
} // End of namespace Sysmel
