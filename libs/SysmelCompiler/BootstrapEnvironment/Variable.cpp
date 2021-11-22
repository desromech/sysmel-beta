#include "sysmel/BootstrapEnvironment/Variable.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/ReferenceType.hpp"
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

AnyValuePtr Variable::getName() const
{
    return name;
}

AnyValuePtr Variable::findStoreBindingInCompileTime(const CompileTimeCleanUpScopePtr &compileTimeCleanUpScope)
{
    (void)compileTimeCleanUpScope;
    return shared_from_this();
}

void Variable::setDefinitionParameters(const AnyValuePtr &definitionName, const TypePtr &definitionValueType, bool definitionMutability, uint64_t definitionMinimalAlignment)
{
    name = definitionName;
    valueType = definitionValueType;
    isMutable_ = definitionMutability;
    minimalAlignment = definitionMinimalAlignment;

    referenceType = valueType;
    if(isMutable_)
    {
        if(valueType->isReferenceLikeType())
        {
            isMutable_ = false;
        }
        else
        {
            referenceType = valueType->ref();
        }
    }
}

ASTNodePtr Variable::analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    auto result = std::make_shared<ASTVariableAccessNode> ();
    result->sourcePosition = node->sourcePosition;
    result->variable = shared_from_this();
    result->isAccessedByReference = isMutable_;
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(result);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius