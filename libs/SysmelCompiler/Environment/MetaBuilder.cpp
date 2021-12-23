#include "Environment/MetaBuilder.hpp"
#include "Environment/ASTNode.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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
        auto selector = node->selector.staticAs<ASTLiteralValueNode> ()->value->asString();
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
    auto newNode = basicMakeObject<ASTCallNode> (*partiallyAnalyzedNode);
    newNode->function = concretizeMetaBuilder();
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(newNode);
}

ASTNodePtr MetaBuilder::concretizeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    auto newNode = basicMakeObject<ASTMessageSendNode> (*partiallyAnalyzedNode);
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
    sysmelAssert(node->value == selfFromThis());
    return concretizeMetaBuilderAndAnalyzeWith(semanticAnalyzer);
}

ASTNodePtr MetaBuilder::concretizeMetaBuilderAndAnalyzeWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(concretizeMetaBuilder());
}

} // End of namespace Environment
} // End of namespace Sysmel
