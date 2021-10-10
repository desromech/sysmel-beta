#include "sysmel/BootstrapEnvironment/Variable.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTIdentifierReferenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVariableAccessNode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<Variable> variableTypeRegistration;

bool Variable::isVariable() const
{
    return true;
}

void Variable::setDefinitionParameters(const AnyValuePtr &definitionName, const TypePtr &definitionValueType, TypeInferenceMode typeInferenceMode, bool definitionMutability, uint64_t definitionMinimalAlignment)
{
    name = definitionName;
    valueType = definitionValueType;
    isMutable_ = definitionMutability;
    minimalAlignment = definitionMinimalAlignment;

    // TODO: Compute this properly.
    referenceType = valueType;
}

ASTNodePtr Variable::analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    auto result = std::make_shared<ASTVariableAccessNode> ();
    result->sourcePosition = node->sourcePosition;
    result->variable = shared_from_this();
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(result);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius