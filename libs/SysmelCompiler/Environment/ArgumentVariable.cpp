#include "Environment/ArgumentVariable.hpp"
#include "Environment/ASTArgumentDefinitionNode.hpp"
#include "Environment/CompileTimeCleanUpScope.hpp"
#include "Environment/Type.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"


namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ArgumentVariable> localVariableTypeRegistration;

bool ArgumentVariable::isArgumentVariable() const
{
    return true;
}

bool ArgumentVariable::isSuperArgument() const
{
    return isSuper;
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

} // End of namespace Environment
} // End of namespace Sysmel