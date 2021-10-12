#include "sysmel/BootstrapEnvironment/CompiledMethod.hpp"
#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/CannotEvaluateMessageInCompileTime.hpp"
#include "sysmel/BootstrapEnvironment/CannotEvaluateUndefinedMessage.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/CompilationError.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTCompileTimeEvaluator.hpp"

#include "sysmel/BootstrapEnvironment/ASTAnalysisEnvironment.hpp"
#include "sysmel/BootstrapEnvironment/CleanUpScope.hpp"
#include "sysmel/BootstrapEnvironment/LexicalScope.hpp"

#include "sysmel/BootstrapEnvironment/FunctionalType.hpp"
#include <assert.h>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<CompiledMethod> compiledMethodTypeRegistration;

bool CompiledMethod::isCompiledMethod() const
{
    return true;
}

bool CompiledMethod::isDefined() const
{
    return definitionEnvironment || analyzedBodyNode;
}

bool CompiledMethod::isDefinedForCompileTime() const
{
    return isDefined();
}

void CompiledMethod::setDeclaration(const ASTNodePtr &node)
{
    declarationNode = node;
    declarationPosition = node->sourcePosition;
}

void CompiledMethod::setDefinition(const ASTNodePtr &node, const ASTNodePtr &bodyNode, const ASTAnalysisEnvironmentPtr &environment)
{
    if(!declarationNode)
        setDeclaration(node);

    definitionNode = node;
    definitionEnvironment = environment;
    definitionPosition = node->sourcePosition;
    definitionBodyNode = bodyNode;
}

ASTAnalysisEnvironmentPtr CompiledMethod::createSemanticAnalysisEnvironment()
{
    auto result = definitionEnvironment->copyWithCleanUpcope(CleanUpScope::makeEmpty());
    result->lexicalScope = LexicalScope::makeWithParent(definitionEnvironment->lexicalScope);
    result->localDefinitionsOwner = shared_from_this();
    return result;
}

void CompiledMethod::ensureSemanticAnalysis()
{
    if(analyzedBodyNode || !isDefined())
        return;

    // FIXME: Use the correct environment here.
    auto analyzer = std::make_shared<ASTSemanticAnalyzer> ();
    analyzer->environment = createSemanticAnalysisEnvironment();
    
    auto analyzedBodyValue = analyzer->visitNode(definitionBodyNode);
    assert(analyzedBodyValue->isASTNode());

    analyzedBodyNode = std::static_pointer_cast<ASTNode> (analyzedBodyValue);

    auto compilationError = analyzer->makeCompilationError();
    if(compilationError)
        compilationError->signal();
}

std::string CompiledMethod::fullPrintString() const
{
    return sexpressionToPrettyString(asFullDefinitionSExpression());
}

SExpression CompiledMethod::asFullDefinitionSExpression() const
{
    if(analyzedBodyNode)
        return analyzedBodyNode->asSExpression();

    return SExpressionVoid{};
}

AnyValuePtr CompiledMethod::runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
{
    (void)selector;
    (void)arguments;
    (void)receiver;
    
    if(!isDefined())
        signalNew<CannotEvaluateUndefinedMessage> ();
    if(!isDefinedForCompileTime())
        signalNew<CannotEvaluateMessageInCompileTime> ();
    ensureSemanticAnalysis();

    return std::make_shared<ASTCompileTimeEvaluator> ()->evaluateMethodBodyNode(analyzedBodyNode);
}

void CompiledMethod::recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild)
{
    children.push_back(newChild);
}

void CompiledMethod::setFunctionalType(const FunctionalTypePtr &newFunctionalType)
{
    functionalType = newFunctionalType;
}

const TypePtr &CompiledMethod::getFunctionalType() const
{
    return functionalType;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius