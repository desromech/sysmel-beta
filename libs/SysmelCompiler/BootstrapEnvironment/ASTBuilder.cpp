#include "sysmel/BootstrapEnvironment/ASTBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainMessageNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTExplicitCastNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTImplicitCastNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTReinterpretCastNode.hpp"
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius