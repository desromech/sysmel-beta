#include "Environment/ValueBox.hpp"
#include "Environment/PointerLikeType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ValueBox> valueBoxTypeRegistration;

bool ValueBox::isValueBox() const
{
    return true;
}

SExpression ValueBox::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{{"valueBox"}},
        type->asSExpression(),
        value ? value->asSExpression() : nullptr
    }};
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

} // End of namespace Environment
} // End of namespace Sysmel
