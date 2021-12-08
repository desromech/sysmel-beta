#include "Environment/AggregateType.hpp"
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

bool AggregateTypeValue::isAggregateTypeValue() const
{
    return true;
}

} // End of namespace Environment
} // End of namespace Sysmel