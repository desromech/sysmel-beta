#include "sysmel/BootstrapEnvironment/ArgumentVariable.hpp"
#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
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

void ArgumentVariable::setDeclarationNode(const ASTArgumentDefinitionNodePtr &node)
{
    name = node->analyzedIdentifier;
}

void ArgumentVariable::setDefinitionNode(const ASTArgumentDefinitionNodePtr &node)
{
    name = node->analyzedIdentifier;
}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius