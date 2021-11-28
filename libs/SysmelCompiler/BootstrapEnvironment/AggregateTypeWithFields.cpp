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

bool AggregateTypeWithFields::canHaveFields() const
{
    return true;
}

void AggregateTypeWithFields::recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild)
{
    SuperType::recordChildProgramEntityDefinition(newChild);
    
    if(newChild->isFieldVariable())
        fields.push_back(staticObjectCast<FieldVariable> (newChild));
}

bool AggregateTypeWithFieldsValue::isAggregateTypeWithFieldsValue() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius