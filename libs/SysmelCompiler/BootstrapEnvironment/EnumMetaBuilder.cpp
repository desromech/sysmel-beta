#include "sysmel/BootstrapEnvironment/EnumMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<EnumMetaBuilder> typeMetaBuilderTypeRegistration;

ASTNodePtr EnumMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->arguments.size() == 1)
    {
        if(selector == "valueType:")
        {
            valueTypeNode = node->arguments[0];
            return node->receiver;
        }
        else if(selector == "values:")
        {
            valuesNode = node->arguments[0];
            return node->receiver;
        }
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius