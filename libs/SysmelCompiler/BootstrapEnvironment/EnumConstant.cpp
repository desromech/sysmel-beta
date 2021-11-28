#include "sysmel/BootstrapEnvironment/EnumConstant.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/MacroInvocationContext.hpp"
#include "sysmel/BootstrapEnvironment/ASTBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTIdentifierReferenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<EnumConstant> compileTimeConstantTypeRegistration;

bool EnumConstant::isEnumConstant() const
{
    return true;
}

AnyValuePtr EnumConstant::getName() const
{
    return name;
}

void EnumConstant::setName(const AnyValuePtr &newName)
{
    name = newName;
}

const AnyValuePtr &EnumConstant::getValue() const
{
    return value;
}

void EnumConstant::setValue(const AnyValuePtr &newValue)
{
    value = newValue;
}

void EnumConstant::setDefinitionNode(const ASTNodePtr &node)
{
    definitionNode = node;
    definitionPosition = node->sourcePosition;
}

ASTNodePtr EnumConstant::analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(
        validAnyValue(value)->asASTNodeRequiredInPosition(node->sourcePosition)
    );
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius