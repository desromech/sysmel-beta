#include "sysmel/BootstrapEnvironment/MetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<MetaBuilder> metaBuilderTypeRegistration;

bool MetaBuilder::isMetaBuilder() const
{
    return true;
}

void MetaBuilder::setMetaBuilderInstanceContext(const MacroInvocationContextPtr &context)
{
    instanceContext = context;
}

ASTNodePtr MetaBuilder::analyzeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    (void)partiallyAnalyzedNode;
    return concretizeMetaBuilderWith(semanticAnalyzer);
}

ASTNodePtr MetaBuilder::concretizeMetaBuilderWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(getVoidConstant()->asASTNodeRequiredInPosition(instanceContext->receiverNode->sourcePosition));
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius
