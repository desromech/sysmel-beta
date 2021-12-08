#include "Environment/EnumConstant.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTLiteralValueNode.hpp"

#include "Environment/ASTIdentifierReferenceNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"


namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel