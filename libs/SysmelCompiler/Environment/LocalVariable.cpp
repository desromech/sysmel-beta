#include "Environment/LocalVariable.hpp"
#include "Environment/CompileTimeCleanUpScope.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<LocalVariable> localVariableTypeRegistration;

bool LocalVariable::isLocalVariable() const
{
    return true;
}

SExpression LocalVariable::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{"localVariable"},
        validAnyValue(name)->asSExpression(),
    }};
}

SExpression LocalVariable::asFullDefinitionSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{"localVariable"},
        validAnyValue(name)->asSExpression(),
        isMutable_,
        valueType->asSExpression(),
        referenceType->asSExpression(),
    }};
}

} // End of namespace Environment
} // End of namespace Sysmel