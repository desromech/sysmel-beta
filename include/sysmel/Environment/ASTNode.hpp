#ifndef SYSMEL_ENVIRONMENT_AST_NODE_HPP
#define SYSMEL_ENVIRONMENT_AST_NODE_HPP
#pragma once

#include "CompilerObject.hpp"
#include <functional>

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSourcePosition);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTVisitor);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTArgumentDefinitionNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTCleanUpScopeNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTClosureNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTCompileTimeEvaluationErrorNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(ASTErrorNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTIdentifierReferenceNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTCallNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTLexicalScopeNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTLiteralValueNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMakeAggregateNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMakeAssociationNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMakeDictionaryNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMakeLiteralArrayNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMakeTupleNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMakeVariantNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMakeVectorNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMessageChainNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMessageChainMessageNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMessageSendNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTParseErrorNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTPragmaNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTQuasiQuoteNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTQuasiUnquoteNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTQuoteNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSequenceNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSemanticErrorNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSpliceNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTVectorSwizzleNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTProgramEntityNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTVariableNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTLocalVariableNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTGlobalVariableNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTFieldVariableNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTCompileTimeConstantNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTFieldVariableAccessNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTVariableAccessNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTLocalImmutableAccessNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSlotAccessNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTFunctionalNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTFunctionNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMethodNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTNamespaceNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTTypeNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTEnumNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTStructNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTUnionNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTClassNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTTemplateNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTProgramEntityExtensionNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTTypeConversionNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTValueAsVoidTypeConversionNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTUpcastTypeConversionNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTDowncastTypeConversionNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTValueAsReferenceReinterpretConversionNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTIfNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTWhileNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTDoWhileNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTReturnNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTContinueNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTBreakNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTCastNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTExplicitCastNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTImplicitCastNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTReinterpretCastNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTQuasiQuotePatternExpansionNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTQuasiQuotePatternExpansionArgumentNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTPatternNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTAlternativesPatternNode); // x | y
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTAnyValuePatternNode); // _
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTBindingPatternNode); // let x [type:] [:= value]
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTNeverPatternNode); // Impossible to match pattern
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTPredicatedPatternNode); // when:
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTRangePatternNode); // to:
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSequencePatternNode);  // x ()
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTValuePatternNode); // Anything else.

typedef std::function<void (const ASTNodePtr &)> ASTIterationBlock;
/**
 * I am the interface for all of the language independent AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTNode : public SubtypeOf<CompilerObject, ASTNode>
{
public:
    static constexpr char const __typeName__[] = "ASTNode";

    ASTNode();

    static MethodCategories __instanceMethods__();

    virtual bool isASTNode() const override;
    virtual bool isASTLiteralSymbolValue() const;
    virtual bool isASTIdentifierSymbolValue() const;
    virtual bool isASTLiteralTypeNode() const;
    virtual bool isPureCompileTimeLiteralValueNode() const;
    virtual bool isPureCompileTimeEvaluableNode() const;

    virtual ASTNodePtr parseAsArgumentNodeWith(const ASTSemanticAnalyzerPtr &semanticAnalyzer);
    virtual ASTNodePtr parseAsPatternNode();
    virtual ASTNodePtr parseAsBindingPatternNode();
    
    virtual ASTNodePtr asASTNodeRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition) override;
    virtual ASTNodePtr asInlinedBlockBodyNode();

    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor);

    virtual std::string printString() const override;

    virtual ASTNodePtrList children();
    virtual void childrenDo(const ASTIterationBlock &aBlock);
    virtual void allChildrenDo(const ASTIterationBlock &aBlock);
    virtual void withAllChildrenDo(const ASTIterationBlock &aBlock);

    ASTSourcePositionPtr sourcePosition;
    TypePtr analyzedType;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_NODE_HPP