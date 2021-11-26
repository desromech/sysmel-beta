#include "sysmel/BootstrapEnvironment/CompileTimeMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/CompileTimeConstantMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius