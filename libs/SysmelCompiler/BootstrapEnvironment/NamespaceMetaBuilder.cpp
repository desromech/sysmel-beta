#include "sysmel/BootstrapEnvironment/NamespaceMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTNamespaceNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<NamespaceMetaBuilder> variableMetaBuilderTypeRegistration;
    
ASTNodePtr NamespaceMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->arguments.size() == 1)
    {
        if(selector == ":=" || selector == "definition:")
        {
            bodyNode = node->arguments[0]->asInlinedBlockBodyNode();
            return concretizeMetaBuilderAndAnalyzeWith(semanticAnalyzer);
        }
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
}

ASTNodePtr NamespaceMetaBuilder::concretizeMetaBuilder()
{
    auto result = std::make_shared<ASTNamespaceNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();

    result->name = nameNode;
    result->body = bodyNode;
    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius