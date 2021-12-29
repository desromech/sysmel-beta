#include "Environment/Variable.hpp"
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/Type.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTIdentifierReferenceNode.hpp"
#include "Environment/ASTVariableAccessNode.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<Variable> variableTypeRegistration;

Variable::Variable()
{
    declarationPosition = ASTSourcePosition::empty();
    definitionPosition = ASTSourcePosition::empty();
}

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
    return selfFromThis();
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

void Variable::setDeclarationNode(const ASTNodePtr &node)
{
    declarationNode = node;
    declarationPosition = node->sourcePosition;
}

const ASTSourcePositionPtr &Variable::getDeclarationPosition() const
{
    return declarationPosition;
}

void Variable::setDefinitionNode(const ASTNodePtr &node)
{
    definitionNode = node;
    definitionPosition = node->sourcePosition;
}

const ASTSourcePositionPtr &Variable::getDefinitionPosition() const
{
    return definitionPosition;
}

ASTNodePtr Variable::analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    auto result = basicMakeObject<ASTVariableAccessNode> ();
    result->sourcePosition = node->sourcePosition;
    result->variable = selfFromThis();
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(result);
}

} // End of namespace Environment
} // End of namespace Sysmel