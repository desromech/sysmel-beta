#include "sysmel/BootstrapEnvironment/VariableMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<VariableMetaBuilder> variableMetaBuilderTypeRegistration;

ASTNodePtr VariableMetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selector, const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node->arguments.empty())
    {
        if(selector == "mutable")
        {
            isMutable = true;
            return node->receiver;
        }

        if(selector == "immutable")
        {
            isMutable = false;
            return node->receiver;
        }

        if(selector == "ref")
        {
            typeInferenceMode = TypeInferenceMode::Reference;
            return node->receiver;
        }

        if(selector == "tempRef")
        {
            typeInferenceMode = TypeInferenceMode::TemporaryReference;
            return node->receiver;            
        }
    }
    else if(node->arguments.size() == 1)
    {
        if(selector == "alignment:")
        {
            alignmentNode = node->arguments[0];
            return node->receiver;
        }
        else if(selector == "type:")
        {
            typeNode = node->arguments[0];
            return node->receiver;
        }
        else if(selector == ":=")
        {
            initialValueNode = node->arguments[0];
            return concretizeMetaBuilderAndAnalyzeWith(semanticAnalyzer);
        }
    }

    return SuperType::analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
}


} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius