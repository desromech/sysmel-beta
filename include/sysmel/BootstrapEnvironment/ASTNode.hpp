#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_NODE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_NODE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSourcePosition);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTVisitor);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTArgumentDefinitionNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTCleanUpScopeNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTClosureNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTCompileTimeEvaluationErrorNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(ASTErrorNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTIdentifierReferenceNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTIntrinsicOperationNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTLexicalScopeNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTLiteralValueNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMakeAssociationNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMakeDictionaryNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMakeLiteralArrayNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMakeTupleNode);
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


/**
 * I am the interface for all of the language independent AST nodes.
 */
class ASTNode : public SubtypeOf<CompilerObject, ASTNode>
{
public:
    static constexpr char const __typeName__[] = "ASTNode";

    ASTNode();

    virtual bool isASTNode() const override;
    virtual bool isASTLiteralSymbolValue() const;
    
    virtual ASTNodePtr asASTNodeRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition) override;

    virtual AnyValuePtr accept(const ASTVisitorPtr &visitor);

    virtual std::string printString() const;

    ASTSourcePositionPtr sourcePosition;
    TypePtr analyzedType;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_NODE_HPP