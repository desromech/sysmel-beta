#include "sysmel/BootstrapEnvironment/NamedProgramEntityMetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<NamedProgramEntityMetaBuilder> namedProgramEntityMetaBuilderTypeRegistration;

ASTNodePtr NamedProgramEntityMetaBuilder::analyzeMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(!nameNode && node->arguments.empty() && isValidNameNode(node->selector))
    {
        nameNode = node->selector;
        return node->receiver;
    }

    return SuperType::analyzeMessageSendNode(node, semanticAnalyzer);
}

bool NamedProgramEntityMetaBuilder::isValidNameNode(const ASTNodePtr &nameNode) const
{
    return nameNode->isASTIdentifierSymbolValue();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius