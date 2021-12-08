#include "Environment/TemplateMetaBuilder.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTTemplateNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<TemplateMetaBuilder> variableMetaBuilderTypeRegistration;

ASTNodePtr TemplateMetaBuilder::analyzeCallNode(const ASTCallNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(!hasParsedCallArguments)
    {
        argumentsNode.reserve(node->arguments.size());
        for(const auto &arg : node->arguments)
            argumentsNode.push_back(arg->parseAsArgumentNodeWith(semanticAnalyzer));
        return node->function;
    }

    return SuperType::analyzeCallNode(node, semanticAnalyzer);
}
    
ASTNodePtr TemplateMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->arguments.size() == 1)
    {
        if(selector == ":=")
        {
            bodyNode = node->arguments[0];
            return concretizeMetaBuilderAndAnalyzeWith(semanticAnalyzer);
        }
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
}

ASTNodePtr TemplateMetaBuilder::concretizeMetaBuilder()
{
    auto result = basicMakeObject<ASTTemplateNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();
    result->visibility = instanceContext->programEntityVisibility;

    result->name = nameNode;
    result->arguments = argumentsNode;
    result->body = bodyNode;
    return result;
}
} // End of namespace Environment
} // End of namespace Sysmel