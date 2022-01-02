#include "Environment/SSAGlobalVariable.hpp"
#include "Environment/SSAValueVisitor.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SSAGlobalVariable> SSAGlobalVariableTypeRegistration;

bool SSAGlobalVariable::isSSAGlobalVariable() const
{
    return true;
}

AnyValuePtr SSAGlobalVariable::accept(const SSAValueVisitorPtr &visitor)
{
    return visitor->visitGlobalVariable(selfFromThis());
}

SExpression SSAGlobalVariable::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"global"}},
        name ? name->asSExpression() : nullptr
    }};
}

SExpression SSAGlobalVariable::asFullDefinitionSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"global"}},
        name ? name->asSExpression() : nullptr,
        valueType ? valueType->asSExpression() : nullptr,
        contentType ? contentType->asSExpression() : nullptr,
        externallyDefined,
        initialValue ? initialValue->asSExpression() : nullptr,
    }};
}

TypePtr SSAGlobalVariable::getValueType() const
{
    return valueType;
}

TypePtr SSAGlobalVariable::getContentType() const
{
    return contentType;
}

void SSAGlobalVariable::setContentAndValueType(const TypePtr &newContentType, const TypePtr &newValueType)
{
    contentType = newContentType;
    valueType = newValueType;
}

const AnyValuePtr &SSAGlobalVariable::getInitialValue()
{
    return initialValue;
}

void SSAGlobalVariable::setInitialValue(const AnyValuePtr &newInitialValue)
{
    initialValue = newInitialValue;
}

bool SSAGlobalVariable::isExternallyDefined() const
{
    return externallyDefined;
}

void SSAGlobalVariable::setExternallyDefined(bool newExternallyDefined)
{
    externallyDefined = newExternallyDefined;
}

} // End of namespace Environment
} // End of namespace Sysmel