#include "sysmel/BootstrapEnvironment/Template.hpp"
#include "sysmel/BootstrapEnvironment/TemplateInstance.hpp"
#include "sysmel/BootstrapEnvironment/ArgumentVariable.hpp"
#include "sysmel/BootstrapEnvironment/ASTCallNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<Template> templateTypeRegistration;

bool Template::isTemplate() const
{
    return true;
}

void Template::recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild)
{
    children.push_back(newChild);
}

AnyValuePtr Template::getName() const
{
    return name;
}

void Template::setName(const AnyValuePtr &newName)
{
    name = newName;
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
        auto argument = std::make_shared<ArgumentVariable> ();
        recordChildProgramEntityDefinition(argument);
        argument->setType(type);
        arguments.push_back(argument);
    }
}

void Template::setArgumentDeclarationNode(size_t index, const ASTArgumentDefinitionNodePtr &argumentNode)
{
    assert(index < arguments.size());
    arguments[index]->setDeclarationNode(argumentNode);
}

void Template::setArgumentDefinitionNode(size_t index, const ASTArgumentDefinitionNodePtr &argumentNode)
{
    assert(index < arguments.size());
    arguments[index]->setDefinitionNode(argumentNode);
}

TemplateInstancePtr Template::getOrCreateTemplateInstanceWithArguments(const AnyValuePtrList &instanceArguments)
{
    assert(instanceArguments.size() == arguments.size());
    auto it = instanceCache.find(instanceArguments);
    if(it != instanceCache.end())
        return it->second;

    auto instance = std::make_shared<TemplateInstance> ();
    instanceCache.insert({instanceArguments, instance});

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
            assert(errorNode);
            return errorNode;
        }

        return validAnyValue(getOrCreateTemplateInstanceValueWithArguments(instanceArguments))->asASTNodeRequiredInPosition(callNode->sourcePosition);
    });

    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(templateInstanceNode);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius