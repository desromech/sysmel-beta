#include "sysmel/BootstrapEnvironment/ClassMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTClassNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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
    auto result = std::make_shared<ASTClassNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();
    result->visibility = instanceContext->programEntityVisibility;

    result->name = nameNode;
    result->body = bodyNode;
    result->superclass = superclassNode;
    return result;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius