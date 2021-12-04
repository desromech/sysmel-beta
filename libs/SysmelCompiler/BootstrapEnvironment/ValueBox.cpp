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
    return selfFromThis();
}

AnyValuePtr ValueBox::accessVariableAsReferenceWithType(const TypePtr &referenceType)
{
    if(referenceType->isReferenceLikeType())
        return referenceType.staticAs<PointerLikeType> ()->makeWithValue(value);
    return value;
}

AnyValuePtr ValueBox::accessVariableAsValueWithType(const TypePtr &valueType)
{
    (void)valueType;
    return value;
}

AnyValuePtr ValueBox::asMutableStoreValue()
{
    auto result = basicMakeObject<ValueBox> ();
    result->value = value;
    result->type = type;
    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius
