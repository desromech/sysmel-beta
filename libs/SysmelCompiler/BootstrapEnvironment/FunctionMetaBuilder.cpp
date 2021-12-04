#include "sysmel/BootstrapEnvironment/FunctionMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTCallNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTFunctionNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<FunctionMetaBuilder> variableMetaBuilderTypeRegistration;

ASTNodePtr FunctionMetaBuilder::analyzeCallNode(const ASTCallNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
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
    
ASTNodePtr FunctionMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->arguments.size() == 1)
    {
        if(selector == "=>" && !resultTypeNode)
        {
            resultTypeNode = node->arguments[0];
            return node->receiver;
        }
        else if(selector == ":=")
        {
            bodyNode = node->arguments[0]->asInlinedBlockBodyNode();
            return concretizeMetaBuilderAndAnalyzeWith(semanticAnalyzer);
        }
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
}

ASTNodePtr FunctionMetaBuilder::concretizeMetaBuilder()
{
    auto result = basicMakeObject<ASTFunctionNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();
    result->visibility = instanceContext->programEntityVisibility;
    result->methodFlags = instanceContext->methodFlags;

    result->name = nameNode;
    result->arguments = argumentsNode;
    result->resultType = resultTypeNode;
    result->body = bodyNode;
    return result;
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius