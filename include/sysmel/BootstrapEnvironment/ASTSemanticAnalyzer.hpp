#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEMANTIC_ANALYZER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEMANTIC_ANALYZER_HPP
#pragma once

#include "ASTVisitor.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTAnalysisEnvironment);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ResultTypeInferenceSlot);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(MacroInvocationContext);

/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTSemanticAnalyzer : public SubtypeOf<ASTVisitor, ASTSemanticAnalyzer>
{
public:
    static constexpr char const __typeName__[] = "ASTSemanticAnalyzer";

    ASTNodePtr recordSemanticErrorInNode(const ASTNodePtr &errorNode, const std::string &message);

    ASTNodePtr analyzeNodeIfNeededWithTypeInference(const ASTNodePtr &node, const ResultTypeInferenceSlotPtr &typeInferenceSlot);
    ASTNodePtr analyzeNodeIfNeededWithExpectedType(const ASTNodePtr &node, const TypePtr &expectedType);
    ASTNodePtr analyzeNodeIfNeededWithAutoType(const ASTNodePtr &node);
    ASTNodePtr analyzeNodeIfNeededWithCurrentExpectedType(const ASTNodePtr &node);

    AnyValuePtr adaptNodeAsMacroArgumentOfType(const ASTNodePtr &node, const TypePtr &expectedType);

    MacroInvocationContextPtr makeMacroInvocationContextFor(const ASTMessageSendNodePtr &node);

    virtual AnyValuePtr visitArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &node);
    virtual AnyValuePtr visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node);
    virtual AnyValuePtr visitClosureNode(const ASTClosureNodePtr &node);
    virtual AnyValuePtr visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node);
    virtual AnyValuePtr visitIntrinsicOperationNode(const ASTIntrinsicOperationNodePtr &node);
    virtual AnyValuePtr visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node);
    virtual AnyValuePtr visitLiteralValueNode(const ASTLiteralValueNodePtr &node);
    virtual AnyValuePtr visitMakeAssociationNode(const ASTMakeAssociationNodePtr &node);
    virtual AnyValuePtr visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node);
    virtual AnyValuePtr visitMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &node);
    virtual AnyValuePtr visitMakeTupleNode(const ASTMakeTupleNodePtr &node);
    virtual AnyValuePtr visitMessageChainNode(const ASTMessageChainNodePtr &node);
    virtual AnyValuePtr visitMessageChainMessageNode(const ASTMessageChainMessageNodePtr &node);
    virtual AnyValuePtr visitMessageSendNode(const ASTMessageSendNodePtr &node);
    virtual AnyValuePtr visitParseErrorNode(const ASTParseErrorNodePtr &node);
    virtual AnyValuePtr visitPragmaNode(const ASTPragmaNodePtr &node);
    virtual AnyValuePtr visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &node);
    virtual AnyValuePtr visitQuasiUnquoteNode(const ASTQuasiUnquoteNodePtr &node);
    virtual AnyValuePtr visitQuoteNode(const ASTQuoteNodePtr &node);
    virtual AnyValuePtr visitSequenceNode(const ASTSequenceNodePtr &node);
    virtual AnyValuePtr visitSpliceNode(const ASTSpliceNodePtr &node);

    ASTAnalysisEnvironmentPtr environment;
    ResultTypeInferenceSlotPtr currentExpectedType;
    ASTErrorNodePtrList recordedErrors;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_SEMANTIC_ANALYZER_HPP