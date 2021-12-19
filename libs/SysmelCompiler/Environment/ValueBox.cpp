#include "Environment/ValueBox.hpp"
#include "Environment/LiteralValueVisitor.hpp"
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

AnyValuePtr ValueBox::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitValueBox(selfFromThis());
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

AnyValuePtr ValueBox::moveAssignValue(const AnyValuePtr &newValue)
{
    value = newValue;
    return selfFromThis();
}

AnyValuePtr ValueBox::accessVariableAsReferenceWithType(const TypePtr &referenceType)
{
    if(referenceType->isReferenceLikeType())
        return referenceType.staticAs<PointerLikeType> ()->makeWithValue(selfFromThis());
    return value;
}

AnyValuePtr ValueBox::accessVariableAsValueWithType(const TypePtr &valueType)
{
    (void)valueType;
    return value;
}

AnyValuePtr ValueBox::asMutableStoreValue()
{
    return shallowClone();
}

} // End of namespace Environment
} // End of namespace Sysmel
