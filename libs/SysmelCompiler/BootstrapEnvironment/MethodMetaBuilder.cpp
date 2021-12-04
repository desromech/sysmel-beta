#include "sysmel/BootstrapEnvironment/MethodMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMethodNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<MethodMetaBuilder> methodMetaBuilderTypeRegistration;

ASTNodePtr MethodMetaBuilder::analyzeMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(!hasAnalyzedMethodPattern)
    {
        hasAnalyzedMethodPattern = true;
        selectorNode = node->selector;
        argumentsNode.reserve(node->arguments.size());
        for(const auto &arg : node->arguments)
            argumentsNode.push_back(arg->parseAsArgumentNodeWith(semanticAnalyzer));
        return node->receiver;
    }

    return SuperType::analyzeMessageSendNode(node, semanticAnalyzer);
}
    
ASTNodePtr MethodMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
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

ASTNodePtr MethodMetaBuilder::concretizeMetaBuilder()
{
    auto result = basicMakeObject<ASTMethodNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();
    result->visibility = instanceContext->programEntityVisibility;
    result->methodFlags = instanceContext->methodFlags;

    result->name = selectorNode;
    result->arguments = argumentsNode;
    result->resultType = resultTypeNode;
    result->body = bodyNode;
    return result;
}
} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius