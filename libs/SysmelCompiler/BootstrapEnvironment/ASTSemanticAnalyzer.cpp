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
#include "sysmel/BootstrapEnvironment/ASTSemanticErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSequenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSpliceNode.hpp"

#include "sysmel/BootstrapEnvironment/MacroInvocationContext.hpp"
#include "sysmel/BootstrapEnvironment/ASTBuilder.hpp"

#include "sysmel/BootstrapEnvironment/ResultTypeInferenceSlot.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/ControlFlowUtilities.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTSemanticAnalyzer> ASTSemanticAnalyzerTypeRegistration;

ASTNodePtr ASTSemanticAnalyzer::recordSemanticErrorInNode(const ASTNodePtr &errorNode, const std::string &message)
{
    auto semanticErrorNode = std::make_shared<ASTSemanticErrorNode> ();
    semanticErrorNode->sourcePosition = errorNode->sourcePosition;
    semanticErrorNode->errorMessage = message;
    semanticErrorNode->analyzedType = Type::getUndefinedType();
    recordedErrors.push_back(semanticErrorNode);
    return semanticErrorNode;
}

MacroInvocationContextPtr ASTSemanticAnalyzer::makeMacroInvocationContextFor(const ASTMessageSendNodePtr &node)
{
    auto result = std::make_shared<MacroInvocationContext> ();
    result->receiverNode = node->receiver;
    result->selfType = Type::getVoidType();
    if(node->receiver && node->receiver->analyzedType)
        result->selfType = node->receiver->analyzedType;
    result->astBuilder = std::make_shared<ASTBuilder> ();
    return result;
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithTypeInference(const ASTNodePtr &node, const ResultTypeInferenceSlotPtr &typeInferenceSlot)
{
    auto oldExpectedType = currentExpectedType;
    currentExpectedType = typeInferenceSlot;
    
    auto analyzedNode = doWithEnsure([&](){
       return analyzeNodeIfNeededWithCurrentExpectedType(node);
    },
    [&](){
        currentExpectedType = oldExpectedType;
    });

    // TODO: Add the implicit cast that might be needed here.
    return analyzedNode;
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithExpectedType(const ASTNodePtr &node, const TypePtr &expectedType)
{
    return analyzeNodeIfNeededWithTypeInference(node, ResultTypeInferenceSlot::makeForType(expectedType));
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithAutoType(const ASTNodePtr &node)
{
    return analyzeNodeIfNeededWithTypeInference(node, ResultTypeInferenceSlot::makeForAuto());
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithCurrentExpectedType(const ASTNodePtr &node)
{
    if(node->analyzedType)
        return node;
    
    auto analysisResult = visitNode(node);
    assert(analysisResult->isASTNode());
    return std::static_pointer_cast<ASTNode> (analysisResult);
}

AnyValuePtr ASTSemanticAnalyzer::adaptNodeAsMacroArgumentOfType(const ASTNodePtr &node, const TypePtr &expectedType)
{
    if(node->isKindOf(expectedType))
        return node;
    
    assert("TODO: Support non-node macro parameters" && false);
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
    auto analyzedNode = std::make_shared<ASTMessageSendNode> (*node);
    analyzedNode->selector = analyzeNodeIfNeededWithAutoType(analyzedNode->selector);

    if(analyzedNode->receiver)
    {
        analyzedNode->receiver = analyzeNodeIfNeededWithAutoType(analyzedNode->receiver);
        return analyzedNode->receiver->analyzedType->analyzeMessageSendNode(analyzedNode, shared_from_this());
    }

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
        pragma = analyzeNodeIfNeededWithExpectedType(pragma, Type::getPragmaType());

    // Analyze the expressions.
    auto voidType = Type::getVoidType();
    if(analyzedNode->expressions.empty())
    {
        analyzedNode->analyzedType = voidType;
    }
    else
    {
        const auto expressionCount = analyzedNode->expressions.size();
        bool hasReturnType = false;

        for(size_t i = 0; i < expressionCount; ++i)
        {
            auto isLast = i + 1 >= expressionCount;
            auto &expression = analyzedNode->expressions[i];

            if(isLast && !hasReturnType)
                expression = analyzeNodeIfNeededWithTypeInference(expression, currentExpectedType);
            else
                expression = analyzeNodeIfNeededWithExpectedType(expression, voidType);
            

        }
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