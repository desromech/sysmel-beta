#include "Environment/AggregateType.hpp"
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