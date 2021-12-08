#include "Environment/CompileTimeConstant.hpp"
#include "Environment/Type.hpp"
#include "Environment/Error.hpp"
#include "Environment/CompilationErrorValue.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

#include "Environment/ASTIdentifierReferenceNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/DeferredCompileTimeCodeFragment.hpp"

#include "Environment/ControlFlowUtilities.hpp"
#include "Environment/StringUtilities.hpp"


namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<CompileTimeConstant> compileTimeConstantTypeRegistration;

bool CompileTimeConstant::isCompileTimeConstant() const
{
    return true;
}

AnyValuePtr CompileTimeConstant::getName() const
{
    return name;
}

void CompileTimeConstant::setName(const AnyValuePtr &newName)
{
    name = newName;
}

void CompileTimeConstant::setDeclarationNode(const ASTNodePtr &node)
{
    declarationNode = node;
    declarationPosition = node->sourcePosition;
}

void CompileTimeConstant::setDefinitionNode(const ASTNodePtr &node)
{
    definitionNode = node;
    definitionPosition = node->sourcePosition;
}

void CompileTimeConstant::ensureSemanticAnalysis()
{
    if(hasEvaluatedValueDefinition)
        return;

    if(isEvaluatingValueDefinition)
        signalNewWithMessage<Error> (formatString("Compile time constant {0} has a cyclic definition for its value.", {{printString()}}));

    isEvaluatingValueDefinition = true;
    value = CompilationErrorValue::uniqueInstance();

    doWithEnsure([&](){
        if(!valueCodeFragment)
            signalNewWithMessage<Error> (formatString("Compile time constant {0} has no definition for its required value.", {{printString()}}));

        if(valueType)
            value = valueCodeFragment->analyzeAndEvaluateWithExpectedType(valueType);
        else
            value = valueCodeFragment->analyzeAndEvaluate();
        valueType = validAnyValue(value)->getType();
    }, [&](){
        isEvaluatingValueDefinition = false;
        hasEvaluatedValueDefinition = true;
    });
}

ASTNodePtr CompileTimeConstant::analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(!hasEvaluatedValueDefinition)
    {
        if(isEvaluatingValueDefinition)
            return semanticAnalyzer->recordSemanticErrorInNode(node, formatString("Compile time constant {0} has a cyclic definition for its value.", {{printString()}}));

        if(!valueCodeFragment)
            return semanticAnalyzer->recordSemanticErrorInNode(node, formatString("Compile time constant {0} has no definition for its required value.", {{printString()}}));
        ensureSemanticAnalysis();
    }

    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(
        validAnyValue(value)->asASTNodeRequiredInPosition(node->sourcePosition)
    );
}

} // End of namespace Environment
} // End of namespace Sysmel