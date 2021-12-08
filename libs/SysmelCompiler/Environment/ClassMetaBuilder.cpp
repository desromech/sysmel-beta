#include "Environment/ClassMetaBuilder.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTClassNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ClassMetaBuilder> classMetaBuilderTypeRegistration;

ASTNodePtr ClassMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->arguments.size() == 1)
    {
        if(selector == "superclass:")
        {
            superclassNode = node->arguments[0];
            return node->receiver;
        }
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
}

ASTNodePtr ClassMetaBuilder::concretizeMetaBuilder()
{
    auto result = basicMakeObject<ASTClassNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();
    result->visibility = instanceContext->programEntityVisibility;

    result->name = nameNode;
    result->body = bodyNode;
    result->superclass = superclassNode;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel