#include "sysmel/BootstrapEnvironment/CompileTimeConstant.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"
#include "sysmel/BootstrapEnvironment/CompilationErrorValue.hpp"
#include "sysmel/BootstrapEnvironment/ReferenceType.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

#include "sysmel/BootstrapEnvironment/ASTIdentifierReferenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/DeferredCompileTimeCodeFragment.hpp"

#include "sysmel/BootstrapEnvironment/ControlFlowUtilities.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"


namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius