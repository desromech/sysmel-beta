#include "Environment/StructMetaBuilder.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTStructNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<StructMetaBuilder> structMetaBuilderTypeRegistration;

ASTNodePtr StructMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(selectorValue == "packed")
    {
        isPacked_ = true;
        return node->receiver;
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selectorValue, node, semanticAnalyzer);
}

ASTNodePtr StructMetaBuilder::concretizeMetaBuilder()
{
    auto result = basicMakeObject<ASTStructNode> ();
    result->sourcePosition = instanceContext->concreteSourcePosition();
    result->visibility = instanceContext->programEntityVisibility;
    result->isPacked = isPacked_;

    result->name = nameNode;
    result->body = bodyNode;
    return result;
}

} // End of namespace Environment
} // End of namespace Sysmel