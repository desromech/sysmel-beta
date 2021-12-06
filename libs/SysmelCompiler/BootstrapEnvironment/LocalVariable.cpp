#include "sysmel/BootstrapEnvironment/LocalVariable.hpp"
#include "sysmel/BootstrapEnvironment/CompileTimeCleanUpScope.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius