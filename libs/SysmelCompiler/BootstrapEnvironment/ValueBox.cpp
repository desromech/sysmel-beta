#include "sysmel/BootstrapEnvironment/ValueBox.hpp"
#include "sysmel/BootstrapEnvironment/PointerLikeType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ValueBox> valueBoxTypeRegistration;

bool ValueBox::isValueBox() const
{
    return true;
}

AnyValuePtr ValueBox::copyAssignValue(const AnyValuePtr &newValue)
{
    value = newValue;
    return shared_from_this();
}

AnyValuePtr ValueBox::accessVariableAsReferenceWithType(const TypePtr &referenceType)
{
    if(referenceType->isReferenceLikeType())
        return std::static_pointer_cast<PointerLikeType> (referenceType)->makeWithValue(value);
    return value;
}

AnyValuePtr ValueBox::accessVariableAsValueWithType(const TypePtr &valueType)
{
    (void)valueType;
    return value;
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius
