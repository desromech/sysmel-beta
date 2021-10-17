#include "sysmel/BootstrapEnvironment/VisibilityMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

#include "sysmel/BootstrapEnvironment/FunctionMetaBuilder.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<VisibilityMetaBuilder> visibilityMetaBuilderTypeRegistration;

void VisibilityMetaBuilder::setMetaBuilderInstanceContext(const MetaBuilderInstanceContextPtr &context)
{
    SuperType::setMetaBuilderInstanceContext(context);
    context->programEntityVisibility = getVisibility();
}

ProgramEntityVisibility VisibilityMetaBuilder::getVisibility() const
{
    return ProgramEntityVisibility::Default;
}

ASTNodePtr VisibilityMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->arguments.empty())
    {
        // Program entitites
        if(selector == "function")
        {
            return delegateToMetaBuilderAt<FunctionMetaBuilder> (node->sourcePosition);
        }
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius