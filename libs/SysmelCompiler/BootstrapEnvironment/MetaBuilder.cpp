#include "sysmel/BootstrapEnvironment/MetaBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCallNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
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

void MetaBuilder::setMetaBuilderInstanceContext(const MetaBuilderInstanceContextPtr &context)
{
    instanceContext = context;
}

ASTNodePtr MetaBuilder::delegateToMetaBuilderAt(const MetaBuilderPtr &delegatedMetaBuilder, const ASTSourcePositionPtr &sourcePosition)
{
    delegatedMetaBuilder->setMetaBuilderInstanceContext(instanceContext);
    
    auto resultNode = delegatedMetaBuilder->asASTNodeRequiredInPosition(sourcePosition);
    resultNode->analyzedType = delegatedMetaBuilder->getType();
    return resultNode;
}

ASTNodePtr MetaBuilder::analyzeCallNode(const ASTCallNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return concretizeCallNode(node, semanticAnalyzer);
}

ASTNodePtr MetaBuilder::analyzeMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(node && node->selector->isASTLiteralSymbolValue())
    {
        auto selector = std::static_pointer_cast<ASTLiteralValueNode> (node->selector)->value->asString();
        return analyzeMessageSendNodeWithSelector(selector, node, semanticAnalyzer);
    }

    return concretizeMessageSendNode(node, semanticAnalyzer);
}

ASTNodePtr MetaBuilder::analyzeMessageSendNodeWithSelector(const std::string &selectorValue, const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    (void)selectorValue;
    return concretizeMessageSendNode(partiallyAnalyzedNode, semanticAnalyzer);
}

ASTNodePtr MetaBuilder::concretizeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    auto newNode = std::make_shared<ASTCallNode> (*partiallyAnalyzedNode);
    newNode->function = concretizeMetaBuilder();
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(newNode);
}

ASTNodePtr MetaBuilder::concretizeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    auto newNode = std::make_shared<ASTMessageSendNode> (*partiallyAnalyzedNode);
    newNode->receiver = concretizeMetaBuilder();
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(newNode);
}

ASTNodePtr MetaBuilder::concretizeMetaBuilder()
{
    return getVoidConstant()->asASTNodeRequiredInPosition(instanceContext->concreteSourcePosition());
}

ASTNodePtr MetaBuilder::concretizeEphemeralCompileTimeObject(const ASTLiteralValueNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    (void)node;
    assert(node->value == shared_from_this());
    return concretizeMetaBuilderAndAnalyzeWith(semanticAnalyzer);
}

ASTNodePtr MetaBuilder::concretizeMetaBuilderAndAnalyzeWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(concretizeMetaBuilder());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius
