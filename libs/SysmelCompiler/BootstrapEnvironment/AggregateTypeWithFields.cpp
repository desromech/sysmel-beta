#include "sysmel/BootstrapEnvironment/AggregateTypeWithFields.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<AggregateTypeWithFieldsValue> AggregateTypeWithFieldsRegistration;

bool AggregateTypeWithFields::isAggregateTypeWithFields() const
{
    return true;
}

bool AggregateTypeWithFieldsValue::isAggregateTypeWithFieldsValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius