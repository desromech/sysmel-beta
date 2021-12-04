#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_BUILDER_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_BUILDER_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTIdentifierReferenceNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMessageChainNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMessageChainMessageNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTExplicitCastNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTImplicitCastNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTReinterpretCastNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTIfNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTWhileNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTDoWhileNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTReturnNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTBreakNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTContinueNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTFieldVariableAccessNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(FieldVariable);

/**
 * I am an utility for building AST nodes.
 */
class ASTBuilder : public SubtypeOf<CompilerObject, ASTBuilder>
{
public:
    static constexpr char const __typeName__[] = "ASTBuilder";

    virtual bool isASTBuilder() const override;

    ASTIdentifierReferenceNodePtr identifier(const AnyValuePtr &identifier);
    ASTIdentifierReferenceNodePtr identifierWithBinding(const AnyValuePtr &identifier, const AnyValuePtr &binding);
    
    ASTLiteralValueNodePtr literal(const AnyValuePtr &value);
    ASTLiteralValueNodePtr literalSymbol(const std::string &symbol);
    ASTMessageChainNodePtr messageChain(const ASTNodePtr &receiver, const ASTNodePtrList &messages);
    ASTMessageChainMessageNodePtr messageChainMessage(const ASTNodePtr &selector, const ASTNodePtrList &arguments);
    ASTMessageSendNodePtr sendToWithArguments(const ASTNodePtr &selector, const ASTNodePtr &receiver, const ASTNodePtrList &arguments);

    ASTExplicitCastNodePtr explicitCastTo(const ASTNodePtr &expression, const ASTNodePtr &targetType);
    ASTImplicitCastNodePtr implicitCastTo(const ASTNodePtr &expression, const ASTNodePtr &targetType);
    ASTReinterpretCastNodePtr reinterpretCastTo(const ASTNodePtr &expression, const ASTNodePtr &targetType);

    ASTIfNodePtr ifThen(const ASTNodePtr &condition, const ASTNodePtr &trueExpression);
    ASTIfNodePtr ifThenElse(const ASTNodePtr &condition, const ASTNodePtr &trueExpression, const ASTNodePtr &falseExpression);

    ASTWhileNodePtr whileDo(const ASTNodePtr &condition, const ASTNodePtr &bodyExpression);
    ASTWhileNodePtr whileDoContinueWith(const ASTNodePtr &condition, const ASTNodePtr &bodyExpression, const ASTNodePtr &continueExpression);

    ASTDoWhileNodePtr doWhile(const ASTNodePtr &bodyExpression, const ASTNodePtr &condition);
    ASTDoWhileNodePtr doWhileContinueWith(const ASTNodePtr &bodyExpression, const ASTNodePtr &condition, const ASTNodePtr &continueExpression);

    ASTReturnNodePtr returnValue(const ASTNodePtr &expression);
    ASTReturnNodePtr returnVoid();
    ASTBreakNodePtr breakThisLoop();
    ASTContinueNodePtr continueThisLoop();

    ASTFieldVariableAccessNodePtr fieldVariableAccess(const ASTNodePtr &aggregate, const FieldVariablePtr &fieldVariable);

    ASTSourcePositionPtr sourcePosition;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_AST_BUILDER_HPP