#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCleanUpScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTClosureNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTIntrinsicOperationNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLexicalScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeAssociationNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeDictionaryNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeLiteralArrayNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeTupleNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainMessageNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTPragmaNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuasiQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuasiUnquoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTParseErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSequenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSpliceNode.hpp"

#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTSemanticAnalyzer> ASTSemanticAnalyzerTypeRegistration;

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeeded(const ASTNodePtr &node)
{
    if(node->analyzedType)
        return node;
    
    auto analysisResult = visitNode(node);
    assert(analysisResult->isASTNode());
    return std::static_pointer_cast<ASTNode> (analysisResult);
}

AnyValuePtr ASTSemanticAnalyzer::visitArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitClosureNode(const ASTClosureNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitIntrinsicOperationNode(const ASTIntrinsicOperationNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitLiteralValueNode(const ASTLiteralValueNodePtr &node)
{
    auto result = std::make_shared<ASTLiteralValueNode> (*node);
    result->analyzedType = node->type;
    return result;
}

AnyValuePtr ASTSemanticAnalyzer::visitMakeAssociationNode(const ASTMakeAssociationNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitMakeTupleNode(const ASTMakeTupleNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitMessageChainNode(const ASTMessageChainNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitMessageChainMessageNode(const ASTMessageChainMessageNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitMessageSendNode(const ASTMessageSendNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitParseErrorNode(const ASTParseErrorNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitPragmaNode(const ASTPragmaNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitQuasiUnquoteNode(const ASTQuasiUnquoteNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitQuoteNode(const ASTQuoteNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitSequenceNode(const ASTSequenceNodePtr &node)
{
    auto analyzedNode = std::make_shared<ASTSequenceNode> (*node);

    // Analyze the pragmas.
    for(auto &pragma : analyzedNode->pragmas)
        pragma = analyzeNodeIfNeeded(pragma);

    // Analyze the expressions.
    if(analyzedNode->expressions.empty())
    {
        // TODO: Set the void type here.
    }
    else
    {
        for(auto &expression : analyzedNode->expressions)
            expression = analyzeNodeIfNeeded(expression);
        analyzedNode->analyzedType = analyzedNode->expressions.back()->analyzedType;
    }

    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitSpliceNode(const ASTSpliceNodePtr &node)
{
    assert(false);
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius