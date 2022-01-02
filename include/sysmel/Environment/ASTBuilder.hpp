#ifndef SYSMEL_ENVIRONMENT_AST_BUILDER_HPP
#define SYSMEL_ENVIRONMENT_AST_BUILDER_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTIdentifierReferenceNode);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSemanticErrorNode);

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
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSlotAccessNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(FieldVariable);

/**
 * I am an utility for building AST nodes.
 */
class SYSMEL_COMPILER_LIB_EXPORT ASTBuilder : public SubtypeOf<CompilerObject, ASTBuilder>
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
    ASTSlotAccessNodePtr slotAccess(const ASTNodePtr &aggregate, uint64_t slotIndex, TypePtr slotReferenceType, bool isNotPaddedSlotIndex = true);
    ASTSlotAccessNodePtr variantSlotAccess(const ASTNodePtr &aggregate, uint64_t slotIndex, TypePtr slotReferenceType, uint64_t typeSelectorSlotIndex, TypePtr typeSelectorSlotReferenceType, uint64_t expectedTypeSelectorValue, bool isNotPaddedSlotIndex = true);

    ASTSemanticErrorNodePtr semanticError(const std::string &message);

    ASTSourcePositionPtr sourcePosition;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_AST_BUILDER_HPP