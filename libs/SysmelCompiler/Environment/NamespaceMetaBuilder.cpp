#include "Environment/NamespaceMetaBuilder.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTNamespaceNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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
    auto result = basicMakeObject<ASTNamespaceNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();

    result->name = nameNode;
    result->body = bodyNode;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel