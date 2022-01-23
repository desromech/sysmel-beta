#include "Environment/LetMetaBuilder.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTDestructuringBindingNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTSequencePatternNode.hpp"
#include "Environment/ASTLocalVariableNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<LetMetaBuilder> letMetaBuilderTypeRegistration;

ASTNodePtr LetMetaBuilder::analyzeCallNode(const ASTCallNodePtr &callNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(!bindingPatternNode)
    {
        auto sequencePattern = basicMakeObject<ASTSequencePatternNode> ();
        sequencePattern->sourcePosition = callNode->sourcePosition;
        sequencePattern->elements.reserve(callNode->arguments.size());
        for(auto &arg : callNode->arguments)
            sequencePattern->elements.push_back(arg->parseAsBindingPatternNode());

        bindingPatternNode = sequencePattern;
        return callNode->function;
    }

    return SuperType::analyzeCallNode(callNode, semanticAnalyzer);

}

ASTNodePtr LetMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(bindingPatternNode)
    {
        if(selectorValue == ":=" && node->arguments.size() == 1)
            initialValueNode = node->arguments[0];

        return concretizeMetaBuilderAndAnalyzeWith(semanticAnalyzer);
    }

    if(selectorValue == "[]" && node->arguments.size() == 1)
    {
        bindingPatternNode = node->arguments[0];
        return node->receiver;
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selectorValue, node, semanticAnalyzer);
}

ASTNodePtr LetMetaBuilder::concretizeMetaBuilder()
{
    if(bindingPatternNode)
    {
        auto destructuringBinding = basicMakeObject<ASTDestructuringBindingNode> ();
        destructuringBinding->sourcePosition = instanceContext->concreteSourcePosition();
        destructuringBinding->patternNode = bindingPatternNode;
        destructuringBinding->valueNode = initialValueNode;
        return destructuringBinding;
    }

    auto result = basicMakeObject<ASTLocalVariableNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();

    result->name = nameNode;
    result->type = typeNode;
    result->alignment = alignmentNode;
    result->initialValue = initialValueNode;
    result->typeInferenceMode = typeInferenceMode;
    result->isMutable = isMutable;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel