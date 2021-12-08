#include "Environment/CompileTimeConstantMetaBuilder.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTCompileTimeConstantNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<CompileTimeConstantMetaBuilder> variableMetaBuilderTypeRegistration;

ASTNodePtr CompileTimeConstantMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->arguments.size() == 1)
    {
        if(selector == "type:")
        {
            typeNode = node->arguments[0];
            return node->receiver;
        }
        else if(selector == ":=")
        {
            valueNode = node->arguments[0];
            return concretizeMetaBuilderAndAnalyzeWith(semanticAnalyzer);
        }
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
}

ASTNodePtr CompileTimeConstantMetaBuilder::concretizeMetaBuilder()
{
    auto result = basicMakeObject<ASTCompileTimeConstantNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();
    result->visibility = instanceContext->programEntityVisibility;

    result->name = nameNode;
    result->type = typeNode;
    result->value = valueNode;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel