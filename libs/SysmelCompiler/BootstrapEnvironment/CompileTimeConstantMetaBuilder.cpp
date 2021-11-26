#include "sysmel/BootstrapEnvironment/CompileTimeConstantMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCompileTimeConstantNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
    auto result = std::make_shared<ASTCompileTimeConstantNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();
    result->visibility = instanceContext->programEntityVisibility;

    result->name = nameNode;
    result->type = typeNode;
    result->value = valueNode;
    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius