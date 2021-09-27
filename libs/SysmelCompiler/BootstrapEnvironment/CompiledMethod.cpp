#include "sysmel/BootstrapEnvironment/CompiledMethod.hpp"
#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTCompileTimeEvaluator.hpp"
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

void CompiledMethod::ensureSemanticAnalysis()
{
    if(analyzedBodyNode || !isDefined())
        return;

    // FIXME: Use the correct environment here.
    auto analyzer = std::make_shared<ASTSemanticAnalyzer> ();
    analyzer->environment = definitionEnvironment;
    
    auto analyzedBodyValue = analyzer->visitNode(definitionBodyNode);
    assert(analyzedBodyValue->isASTNode());

    analyzedBodyNode = std::static_pointer_cast<ASTNode> (analyzedBodyValue);
}

AnyValuePtr CompiledMethod::runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
{
    (void)selector;
    (void)arguments;
    (void)receiver;
    
    if(!isDefined())
        throw CannotEvaluateUndefinedMessage();
    if(!isDefinedForCompileTime())
        throw CannotEvaluateMessageInCompileTime();
    ensureSemanticAnalysis();

    auto evaluator = std::make_shared<ASTCompileTimeEvaluator> ();
    return evaluator->visitNode(analyzedBodyNode);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius