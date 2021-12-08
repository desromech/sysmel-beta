#include "Environment/AggregateTypeWithFields.hpp"
#include "Environment/FieldVariable.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <sstream>

namespace Sysmel
{
namespace Environment
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
} // End of namespace Environment
} // End of namespace Sysmel