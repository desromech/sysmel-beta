#include "sysmel/BootstrapEnvironment/ASTBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainMessageNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTExplicitCastNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTImplicitCastNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTReinterpretCastNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTIfNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTWhileNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTDoWhileNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTReturnNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTContinueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTBreakNode.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTBuilder> ASTBuilderTypeRegistration;

bool ASTBuilder::isASTBuilder() const
{
    return true;
}

ASTLiteralValueNodePtr ASTBuilder::literal(const AnyValuePtr &value)
{
    auto node = std::make_shared<ASTLiteralValueNode> ();
    node->sourcePosition = sourcePosition;
    node->value = value;
    node->type = validAnyValue(value)->getType();
    return node;
}

ASTLiteralValueNodePtr ASTBuilder::literalSymbol(const std::string &symbol)
{
    return literal(internSymbol(symbol));
}

ASTMessageChainNodePtr ASTBuilder::messageChain(const ASTNodePtr &receiver, const ASTNodePtrList &messages)
{
    auto node = std::make_shared<ASTMessageChainNode> ();
    node->sourcePosition = sourcePosition;
    node->receiver = receiver;
    node->messages = messages;
    return node;
}

ASTMessageChainMessageNodePtr ASTBuilder::messageChainMessage(const ASTNodePtr &selector, const ASTNodePtrList &arguments)
{
    auto node = std::make_shared<ASTMessageChainMessageNode> ();
    node->sourcePosition = sourcePosition;
    node->selector = selector;
    node->arguments = arguments;
    return node;
}

ASTMessageSendNodePtr ASTBuilder::sendToWithArguments(const ASTNodePtr &selector, const ASTNodePtr &receiver, const ASTNodePtrList &arguments)
{
    auto node = std::make_shared<ASTMessageSendNode> ();
    node->sourcePosition = sourcePosition;
    node->selector = selector;
    node->receiver = receiver;
    node->arguments = arguments;
    return node;
}

ASTExplicitCastNodePtr ASTBuilder::explicitCastTo(const ASTNodePtr &expression, const ASTNodePtr &targetType)
{
    auto node = std::make_shared<ASTExplicitCastNode> ();
    node->sourcePosition = sourcePosition;
    node->expression = expression;
    node->targetType = targetType;
    return node;
}

ASTImplicitCastNodePtr ASTBuilder::implicitCastTo(const ASTNodePtr &expression, const ASTNodePtr &targetType)
{
    auto node = std::make_shared<ASTImplicitCastNode> ();
    node->sourcePosition = sourcePosition;
    node->expression = expression;
    node->targetType = targetType;
    return node;
}

ASTReinterpretCastNodePtr ASTBuilder::reinterpretCastTo(const ASTNodePtr &expression, const ASTNodePtr &targetType)
{
    auto node = std::make_shared<ASTReinterpretCastNode> ();
    node->sourcePosition = sourcePosition;
    node->expression = expression;
    node->targetType = targetType;
    return node;
}

ASTIfNodePtr ASTBuilder::ifThen(const ASTNodePtr &condition, const ASTNodePtr &trueExpression)
{
    auto node = std::make_shared<ASTIfNode> ();
    node->sourcePosition = sourcePosition;
    node->condition = condition;
    node->trueExpression = trueExpression;
    return node;
}

ASTIfNodePtr ASTBuilder::ifThenElse(const ASTNodePtr &condition, const ASTNodePtr &trueExpression, const ASTNodePtr &falseExpression)
{
    auto node = std::make_shared<ASTIfNode> ();
    node->sourcePosition = sourcePosition;
    node->condition = condition;
    node->trueExpression = trueExpression;
    node->falseExpression = falseExpression;
    return node;
}

ASTWhileNodePtr ASTBuilder::whileDo(const ASTNodePtr &condition, const ASTNodePtr &bodyExpression)
{
    auto node = std::make_shared<ASTWhileNode> ();
    node->sourcePosition = sourcePosition;
    node->condition = condition;
    node->bodyExpression = bodyExpression;
    return node;
}

ASTWhileNodePtr ASTBuilder::whileDoContinueWith(const ASTNodePtr &condition, const ASTNodePtr &bodyExpression, const ASTNodePtr &continueExpression)
{
    auto node = std::make_shared<ASTWhileNode> ();
    node->sourcePosition = sourcePosition;
    node->condition = condition;
    node->bodyExpression = bodyExpression;
    node->continueExpression = continueExpression;
    return node;
}

ASTDoWhileNodePtr ASTBuilder::doWhile(const ASTNodePtr &bodyExpression, const ASTNodePtr &condition)
{
    auto node = std::make_shared<ASTDoWhileNode> ();
    node->sourcePosition = sourcePosition;
    node->bodyExpression = bodyExpression;
    node->condition = condition;
    return node;
}

ASTDoWhileNodePtr ASTBuilder::doWhileContinueWith(const ASTNodePtr &bodyExpression,  const ASTNodePtr &condition, const ASTNodePtr &continueExpression)
{
    auto node = std::make_shared<ASTDoWhileNode> ();
    node->sourcePosition = sourcePosition;
    node->bodyExpression = bodyExpression;
    node->condition = condition;
    node->continueExpression = continueExpression;
    return node;
}

ASTReturnNodePtr ASTBuilder::returnValue(const ASTNodePtr &expression)
{
    auto node = std::make_shared<ASTReturnNode> ();
    node->sourcePosition = sourcePosition;
    node->expression = expression;
    return node;
}

ASTReturnNodePtr ASTBuilder::returnVoid()
{
    auto node = std::make_shared<ASTReturnNode> ();
    node->sourcePosition = sourcePosition;
    return node;
}

ASTBreakNodePtr ASTBuilder::breakThisLoop()
{
    auto node = std::make_shared<ASTBreakNode> ();
    node->sourcePosition = sourcePosition;
    return node;
}

ASTContinueNodePtr ASTBuilder::continueThisLoop()
{
    auto node = std::make_shared<ASTContinueNode> ();
    node->sourcePosition = sourcePosition;
    return node;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius