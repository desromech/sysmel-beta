#include "sysmel/BootstrapEnvironment/AggregateType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius