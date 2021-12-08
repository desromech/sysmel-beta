#include "Environment/CompileTimeMetaBuilder.hpp"
#include "Environment/CompileTimeConstantMetaBuilder.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<CompileTimeMetaBuilder> visibilityMetaBuilderTypeRegistration;

void CompileTimeMetaBuilder::setMetaBuilderInstanceContext(const MetaBuilderInstanceContextPtr &context)
{
    SuperType::setMetaBuilderInstanceContext(context);
    context->isCompileTime = true;
}

ASTNodePtr CompileTimeMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->arguments.empty())
    {
        // Functional
        if(selector == "constant")
            return delegateToMetaBuilderAt<CompileTimeConstantMetaBuilder> (node->sourcePosition);
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
}


} // End of namespace Environment
} // End of namespace Sysmel