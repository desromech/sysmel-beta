#include "sysmel/BootstrapEnvironment/TemplateInstance.hpp"
#include "sysmel/BootstrapEnvironment/Template.hpp"
#include "sysmel/BootstrapEnvironment/ASTAnalysisEnvironment.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/CompilationError.hpp"
#include "sysmel/BootstrapEnvironment/LexicalScope.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<TemplateInstance> templateInstanceTypeRegistration;

bool TemplateInstance::isTemplateInstance() const
{
    return true;
}

void TemplateInstance::recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild)
{
    children.push_back(newChild);
    newChild->setParentProgramEntity(selfFromThis());
}

const AnyValuePtr &TemplateInstance::getValue() const
{
    return value;
}

void TemplateInstance::addArgumentBinding(const AnyValuePtr &name, const AnyValuePtr &value)
{
    if(validAnyValue(name)->isAnonymousNameSymbol())
        return;

    argumentBindings.push_back({name, value});
}

void TemplateInstance::evaluateDefinitionFragment(const TemplateDefinitionFragment &fragment)
{
    auto scope = LexicalScope::makeWithParent(fragment.environment->lexicalScope);
    for(auto &[key, value] : argumentBindings)
        scope->setSymbolBinding(key, validAnyValue(value));

    auto evaluationEnvironment = fragment.environment->copyWithLexicalScope(scope);
    auto analyzer = basicMakeObject<ASTSemanticAnalyzer> ();
    analyzer->environment = evaluationEnvironment;

    auto analyzedBody = analyzer->analyzeNodeIfNeededWithAutoType(fragment.bodyNode);
    auto error = analyzer->makeCompilationError();
    if(error)
        error->signal();

    auto evaluatedResult = analyzer->evaluateInCompileTime(analyzedBody);
    if(!hasEvaluatedMainDefinitionFragment)
    {
        value = evaluatedResult;
        hasEvaluatedMainDefinitionFragment = true;
    }
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius