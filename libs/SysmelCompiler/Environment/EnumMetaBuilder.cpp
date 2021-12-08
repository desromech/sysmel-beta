#include "Environment/EnumMetaBuilder.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTEnumNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<EnumMetaBuilder> typeMetaBuilderTypeRegistration;

ASTNodePtr EnumMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->arguments.size() == 1)
    {
        if(selector == "valueType:")
        {
            valueTypeNode = node->arguments[0];
            return node->receiver;
        }
        else if(selector == "values:")
        {
            valuesNode = node->arguments[0];
            return node->receiver;
        }
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
}

ASTNodePtr EnumMetaBuilder::concretizeMetaBuilder()
{
    auto result = basicMakeObject<ASTEnumNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();
    result->visibility = instanceContext->programEntityVisibility;

    result->name = nameNode;
    result->body = bodyNode;
    result->valueType = valueTypeNode;
    result->values = valuesNode;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel