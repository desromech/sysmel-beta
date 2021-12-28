#include "Environment/Template.hpp"
#include "Environment/TemplateInstance.hpp"
#include "Environment/ArgumentVariable.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/SSATemplate.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<Template> templateTypeRegistration;

bool Template::isTemplate() const
{
    return true;
}

void Template::recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild)
{
    children.push_back(newChild);
    newChild->setParentProgramEntity(selfFromThis());
}

AnyValuePtr Template::getName() const
{
    return name;
}

void Template::setName(const AnyValuePtr &newName)
{
    name = newName;
}

SSAValuePtr Template::asSSAValueRequiredInPosition(const ASTSourcePositionPtr &)
{
    if(!ssaTemplate)
    {
        ssaTemplate = basicMakeObject<SSATemplate> ();
        ssaTemplate->setName(getValidName());
        ssaTemplate->setExternalLanguageMode(externalLanguageMode);
        ssaTemplate->setVisibility(visibility);
        ssaTemplate->setDllLinkageMode(dllLinkageMode);
        auto ssaParent = getParentProgramEntity()->asProgramEntitySSAValue();
        if(ssaParent)
        {
            sysmelAssert(ssaParent->isSSAProgramEntity());
            ssaParent.staticAs<SSAProgramEntity>()->addChild(ssaTemplate);
        }
    }

    return ssaTemplate;
}

void Template::setDeclarationNode(const ASTNodePtr &node)
{
    declarationPosition = node->sourcePosition;
    declarationNode = node;
}

void Template::addDefinition(const ASTNodePtr &node, const ASTNodePtr &bodyNode, const ASTAnalysisEnvironmentPtr &bodyEnvironment)
{
    definitionFragments.push_back({
        node->sourcePosition,
        node,
        bodyNode,
        bodyEnvironment
    });
}

void Template::setArgumentTypes(const TypePtrList &argumentTypes)
{
    arguments.reserve(argumentTypes.size());
    for(const auto &type : argumentTypes)
    {
        auto argument = basicMakeObject<ArgumentVariable> ();
        recordChildProgramEntityDefinition(argument);
        argument->setType(type);
        arguments.push_back(argument);
    }
}

void Template::setArgumentDeclarationNode(size_t index, const ASTArgumentDefinitionNodePtr &argumentNode)
{
    sysmelAssert(index < arguments.size());
    arguments[index]->setArgumentDeclarationNode(argumentNode);
}

void Template::setArgumentDefinitionNode(size_t index, const ASTArgumentDefinitionNodePtr &argumentNode)
{
    sysmelAssert(index < arguments.size());
    arguments[index]->setArgumentDefinitionNode(argumentNode);
}

TemplateInstancePtr Template::getOrCreateTemplateInstanceWithArguments(const AnyValuePtrList &instanceArguments)
{
    sysmelAssert(instanceArguments.size() == arguments.size());
    auto it = instanceCache.find(instanceArguments);
    if(it != instanceCache.end())
        return it->second;

    auto instance = basicMakeObject<TemplateInstance> ();
    instanceCache.insert({instanceArguments, instance});

    // TODO: Record this on a module specific program entity if needed.
    recordChildProgramEntityDefinition(instance);

    for(size_t i = 0; i < instanceArguments.size(); ++i)
        instance->addArgumentBinding(arguments[i]->getName(), instanceArguments[i]);

    for(auto &fragment : definitionFragments)
        instance->evaluateDefinitionFragment(fragment);

    return instance;
}

AnyValuePtr Template::getOrCreateTemplateInstanceValueWithArguments(const AnyValuePtrList &instanceArguments)
{
    return getOrCreateTemplateInstanceWithArguments(instanceArguments)->getValue();
}

ASTNodePtr Template::analyzeCallNode(const ASTCallNodePtr &callNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(callNode->arguments.size() != arguments.size())
        return semanticAnalyzer->recordSemanticErrorInNode(callNode, formatString("Template {0} expects {1} arguments instead of the {2} provided arguments.",
            {printString(), castToString(arguments.size()), castToString(callNode->arguments.size())}));

    if(definitionFragments.empty())
        return semanticAnalyzer->recordSemanticErrorInNode(callNode, formatString("Cannot instance template {0} without a definition body.", {printString()}));

    auto templateInstanceNode = semanticAnalyzer->guardCompileTimeEvaluationForNode(callNode, [&](){
        AnyValuePtrList instanceArguments;
        instanceArguments.reserve(arguments.size());
        bool hasError = false;
        ASTNodePtr errorNode;
        for(size_t i = 0; i < arguments.size(); ++i)
        {
            auto expectedType = arguments[i]->getValueType();
            auto argumentNode = semanticAnalyzer->analyzeNodeIfNeededWithExpectedType(callNode->arguments[i], expectedType);
            if(argumentNode->isASTErrorNode())
            {
                if(!hasError)
                    errorNode = argumentNode;
                hasError = true;
                continue;
            }

            auto argumentValue = semanticAnalyzer->evaluateInCompileTime(argumentNode);
            if(validAnyValue(argumentValue)->isCompilationErrorValue())
            {
                if(!hasError)
                    errorNode = semanticAnalyzer->recordSemanticErrorInNode(callNode, "Failed to evaluate template instance argument.");

                hasError = true;
                continue;
            }

            instanceArguments.push_back(argumentValue);
        }

        if(hasError)
        {
            sysmelAssert(errorNode);
            return errorNode;
        }

        return validAnyValue(getOrCreateTemplateInstanceValueWithArguments(instanceArguments))->asASTNodeRequiredInPosition(callNode->sourcePosition);
    });

    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(templateInstanceNode);
}

} // End of namespace Environment
} // End of namespace Sysmel