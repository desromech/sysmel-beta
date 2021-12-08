#include "Environment/NamedProgramEntityMetaBuilder.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel