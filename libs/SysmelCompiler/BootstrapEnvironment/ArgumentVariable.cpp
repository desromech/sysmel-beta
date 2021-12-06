#include "sysmel/BootstrapEnvironment/ArgumentVariable.hpp"
#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/CompileTimeCleanUpScope.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ArgumentVariable> localVariableTypeRegistration;

bool ArgumentVariable::isArgumentVariable() const
{
    return true;
}

void ArgumentVariable::setType(const TypePtr &type)
{
    valueType = type;
    referenceType = type;
}


void ArgumentVariable::setName(const AnyValuePtr &newName)
{
    name = newName;
}

void ArgumentVariable::setArgumentDeclarationNode(const ASTArgumentDefinitionNodePtr &node)
{
    name = node->analyzedIdentifier;
    setDeclarationNode(node);
}

void ArgumentVariable::setArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &node)
{
    name = node->analyzedIdentifier;
    setDefinitionNode(node);
}

SExpression ArgumentVariable::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{"argumentVariable"},
        validAnyValue(name)->asSExpression(),
    }};
}

SExpression ArgumentVariable::asFullDefinitionSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{"argumentVariable"},
        validAnyValue(name)->asSExpression(),
        valueType->asSExpression(),
    }};
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius