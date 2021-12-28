#include "Environment/BootstrapFieldVariable.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/SubclassResponsibility.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"


namespace Sysmel
{
namespace Environment
{

/**
 * I am an accessor used for plugin bootstrap type references.
 */
class BootstrapFieldReferenceAccessor : public SubtypeOf<CompilerObject, BootstrapFieldReferenceAccessor>
{
public:
    virtual AnyValuePtr copyAssignValue(const AnyValuePtr &newValue) override
    {
        fieldVariable->storeFieldValueIn(newValue, receiver);
        return selfFromThis();
    }
    
    virtual AnyValuePtr accessVariableAsReferenceWithType(const TypePtr &referenceType) override
    {
        if(referenceType->isReferenceLikeType())
            return referenceType.staticAs<PointerLikeType> ()->makeWithValue(selfFromThis());
        return fieldVariable->loadFieldFrom(receiver);
    }

    virtual AnyValuePtr accessVariableAsValueWithType(const TypePtr &valueType) override
    {
        (void)valueType;
        return fieldVariable->loadFieldFrom(receiver);
    }

    virtual AnyValuePtr asMutableStoreValue() override
    {
        return validAnyValue(fieldVariable->loadFieldFrom(receiver))->asMutableStoreValue();
    }

    AnyValuePtr receiver;
    BootstrapFieldVariableBasePtr fieldVariable;
};


static BootstrapTypeRegistration<BootstrapFieldReferenceAccessor> bootstrapFieldReferenceAccessorTypeRegistration;
static BootstrapTypeRegistration<BootstrapFieldVariableBase> bootstrapFieldVariableBaseTypeRegistration;

BootstrapFieldVariableBase::BootstrapFieldVariableBase()
{
    declarationPosition = ASTSourcePosition::empty();
    definitionPosition = ASTSourcePosition::empty();
}

bool BootstrapFieldVariableBase::isBootstrapFieldVariable() const
{
    return true;
}

SExpression BootstrapFieldVariableBase::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{"bootstrapFieldVariable"},
        validAnyValue(name)->asSExpression(),
    }};
}

SExpression BootstrapFieldVariableBase::asFullDefinitionSExpression() const
{
    return asSExpression();
}

AnyValuePtr BootstrapFieldVariableBase::loadFieldFrom(const AnyValuePtr &)
{
    SysmelSelfSubclassResponsibility();
}

void BootstrapFieldVariableBase::storeFieldValueIn(const AnyValuePtr &, const AnyValuePtr &)
{
    SysmelSelfSubclassResponsibility();
}

AnyValuePtr BootstrapFieldVariableBase::getBootstrapFieldReferenceForWithType(const AnyValuePtr &receiver, const TypePtr &expectedReferenceType)
{
    sysmelAssert(expectedReferenceType->isPointerLikeType());
    auto accessor = basicMakeObject<BootstrapFieldReferenceAccessor> ();
    accessor->receiver = receiver;
    accessor->fieldVariable = selfFromThis();

    return expectedReferenceType.staticAs<PointerLikeType>()->makeWithValue(accessor);

}

} // End of namespace Environment
} // End of namespace Sysmel