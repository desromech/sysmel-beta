#include "Environment/TemplateInstance.hpp"
#include "Environment/Template.hpp"
#include "Environment/ASTAnalysisEnvironment.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/CompilationError.hpp"
#include "Environment/LexicalScope.hpp"
#include "Environment/SSATemplateInstance.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

SSAValuePtr TemplateInstance::asSSAValueRequiredInPosition(const ASTSourcePositionPtr &)
{
    if(!ssaTemplateInstance)
    {
        ssaTemplateInstance = basicMakeObject<SSATemplateInstance> ();
        ssaTemplateInstance->setExternalLanguageMode(externalLanguageMode);
        ssaTemplateInstance->setVisibility(visibility);
        ssaTemplateInstance->setDllLinkageMode(dllLinkageMode);
        auto ssaParent = getParentProgramEntity()->asProgramEntitySSAValue();

        AnyValuePtrList arguments;
        arguments.reserve(argumentBindings.size());
        for(auto &binding : argumentBindings)
            arguments.push_back(binding.second);
        ssaTemplateInstance->setArguments(arguments);

        if(ssaParent)
        {
            sysmelAssert(ssaParent->isSSAProgramEntity());
            ssaParent.staticAs<SSAProgramEntity>()->addChild(ssaTemplateInstance);
        }
    }

    return ssaTemplateInstance;
}

const AnyValuePtr &TemplateInstance::getValue() const
{
    return value;
}

void TemplateInstance::addArgumentBinding(const AnyValuePtr &name, const AnyValuePtr &argumentValue)
{
    if(validAnyValue(name)->isAnonymousNameSymbol())
        return;

    argumentBindings.push_back({name, argumentValue});
}

void TemplateInstance::evaluateDefinitionFragment(const TemplateDefinitionFragment &fragment)
{
    auto scope = LexicalScope::makeWithParent(fragment.environment->lexicalScope);
    for(auto &[key, argumentValue] : argumentBindings)
        scope->setSymbolBinding(key, validAnyValue(argumentValue));

    auto evaluationEnvironment = fragment.environment->copyWithLexicalScope(scope);
    evaluationEnvironment->programEntityForPublicDefinitions = selfFromThis();
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

} // End of namespace Environment
} // End of namespace Sysmel