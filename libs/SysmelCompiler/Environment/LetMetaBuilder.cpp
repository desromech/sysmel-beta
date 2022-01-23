#include "Environment/LetMetaBuilder.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTDestructuringBindingNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTLocalVariableNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<LetMetaBuilder> letMetaBuilderTypeRegistration;

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