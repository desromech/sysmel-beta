#include "sysmel/BootstrapEnvironment/AggregateTypeWithFields.hpp"
#include "sysmel/BootstrapEnvironment/FieldVariable.hpp"
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

bool AggregateTypeWithFields::canHaveUserDefinedFields() const
{
    return true;
}

bool AggregateTypeWithFieldsValue::isAggregateTypeWithFieldsValue() const
{
    return true;
}

AnyValuePtr AggregateTypeWithFieldsValue::getReferenceToFieldWithType(const FieldVariablePtr &field, const TypePtr &referenceType)
{
    if(field->isBootstrapFieldVariable())
        SuperType::getReferenceToFieldWithType(field, referenceType);
    return getReferenceToSlotWithType(field->getSlotIndex(), referenceType);
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius