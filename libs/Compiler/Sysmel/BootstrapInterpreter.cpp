#include "sysmel/Compiler/Sysmel/BootstrapInterpreter.hpp"
#include "sysmel/ObjectModel/ObjectEnvironmentBuilders.hpp"

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{

using namespace ObjectModel;

class BootstrapInterpreterVisitor : public ASTVisitor
{
public:
    virtual std::any visitParseErrorNode(ASTParseErrorNode &) { abort(); }

    virtual std::any visitExpressionListNode(ASTExpressionListNode &list)
    {
        ObjectPtr result = UndefinedObject::uniqueInstance();
        for(auto &expr : list.expressions)
            result = std::any_cast<ObjectPtr> (visitNode(*expr));

        return result;
    }

    virtual std::any visitPragmaNode(ASTPragmaNode &)  { abort(); };
    virtual std::any visitBlockNode(ASTBlockNode &)  { abort(); };
    virtual std::any visitBlockClosureArgumentNode(ASTBlockClosureArgumentNode &)  { abort(); };
    virtual std::any visitBlockClosureSignatureNode(ASTBlockClosureSignatureNode &)  { abort(); };

    virtual std::any visitIntegerLiteralNode(ASTIntegerLiteralNode &node)
    {
        ObjectPtr result;
        if(node.value.value < 0)
            result = std::make_shared<LiteralSignedInteger> (node.value.value);
        else
            result = std::make_shared<LiteralUnsignedInteger> (node.value.value);
        return result;
    }

    virtual std::any visitFloatLiteralNode(ASTFloatLiteralNode &node)
    {
        return ObjectPtr(std::make_shared<LiteralFloat> (node.value));
    }

    virtual std::any visitCharacterLiteralNode(ASTCharacterLiteralNode &node)
    {
        return ObjectPtr(std::make_shared<LiteralCharacter> (node.value));
    }

    virtual std::any visitStringLiteralNode(ASTStringLiteralNode &node)
    {
        return ObjectPtr(std::make_shared<LiteralString> (node.value));
    }

    virtual std::any visitSymbolLiteralNode(ASTSymbolLiteralNode &node)
    {
        return ObjectPtr(std::make_shared<LiteralSymbol> (node.value));
    }

    virtual std::any visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node)
    {
        auto binding = currentScope->lookSymbolRecursively(node.identifier);
        if(!binding.has_value())
            throw std::runtime_error("Failed to find symbol binding for " + node.identifier + ".");

        return binding.value();
    }

    ObjectPtr evaluateMessageSendTo(const ObjectPtr &receiver, const ASTNodePtr &selectorNode, const ASTNodePtrList &argumentNodes, const ASTNode &position)
    {
        (void)position;
        auto selector = std::any_cast<ObjectPtr> (visitNode(*selectorNode));
        if(!selector->isSymbol())
            throw std::runtime_error("Message send selector must be a symbol.");
        auto selectorValue = selector->asNativeString();

        ObjectPtrList arguments;
        arguments.reserve(argumentNodes.size());
        for(auto &arg : argumentNodes)
        {
            arguments.push_back(std::any_cast<ObjectPtr> (visitNode(*arg)));
        }

        return receiver->performWithArguments(selectorValue, arguments);

    }
    virtual std::any visitMessageSendNode(ASTMessageSendNode &node)
    {
        if(!node.receiver)
            throw std::runtime_error("Unsupported message without receiver.");

        auto receiver = std::any_cast<ObjectPtr> (visitNode(*node.receiver));
        return evaluateMessageSendTo(receiver, node.selector, node.arguments, node);
    }

    virtual std::any visitMessageChainNode(ASTMessageChainNode &node)
    {
        if(!node.receiver)
            throw std::runtime_error("Unsupported message without receiver.");

        auto receiver = std::any_cast<ObjectPtr> (visitNode(*node.receiver));
        auto result = receiver;
        for(auto &message : node.messages)
        {
            auto messageNode = std::static_pointer_cast<ASTMessageChainMessageNode> (message);
            result = evaluateMessageSendTo(receiver, messageNode->selector, messageNode->arguments, *messageNode);
        }

        return result;
    }

    virtual std::any visitMessageChainMessageNode(ASTMessageChainMessageNode &)  { abort(); };
    virtual std::any visitCallNode(ASTCallNode &)  { abort(); };
    virtual std::any visitSubscriptNode(ASTSubscriptNode &)  { abort(); };

    virtual std::any visitMakeTupleNode(ASTMakeTupleNode &)  { abort(); };
    virtual std::any visitMakeDictionaryNode(ASTMakeDictionaryNode &)  { abort(); };
    virtual std::any visitDictionaryElementNode(ASTDictionaryElementNode &)  { abort(); };
    virtual std::any visitLiteralArrayNode(ASTLiteralArrayNode &)  { abort(); };

    virtual std::any visitQuoteNode(ASTQuoteNode &)  { abort(); };
    virtual std::any visitQuasiQuoteNode(ASTQuasiQuoteNode &)  { abort(); };
    virtual std::any visitQuasiUnquoteNode(ASTQuasiUnquoteNode &)  { abort(); };
    virtual std::any visitSpliceNode(ASTSpliceNode &)  { abort(); };

    BootstrapEvaluationEnvironmentPtr currentScope;
};

BootstrapEvaluationEnvironmentPtr createDefaultBootstrapEvaluationEnvironmentFor(const ObjectModel::ObjectEnvironmentPtr &objectEnvironment)
{
    auto env = std::make_shared<BootstrapLexicalScopeEnvironment> (nullptr);
    env->setSymbol("nil", ObjectModel::UndefinedObject::uniqueInstance());
    env->setSymbol("void", ObjectModel::LiteralVoid::uniqueInstance());
    env->setSymbol("true", ObjectModel::LiteralTrue::uniqueInstance());
    env->setSymbol("false", ObjectModel::LiteralFalse::uniqueInstance());

    auto typeMetaBuilderFactory = std::make_shared<ObjectEnvironmentTypeMetaBuilder> ();
    typeMetaBuilderFactory->objectEnvironment = objectEnvironment;
    env->setSymbol("__compilationTarget__", objectEnvironment->getCompilationTarget());
    env->setSymbol("__type", typeMetaBuilderFactory);

    return std::make_shared<BootstrapSchemaTypesEnvironment> (env, objectEnvironment->getSchema());
}

ObjectModel::ObjectPtr evaluateBootstrapScriptAST(const ASTNodePtr &ast, const BootstrapEvaluationEnvironmentPtr &evaluationEnvironment)
{
    auto evaluator = std::make_shared<BootstrapInterpreterVisitor> ();
    evaluator->currentScope = evaluationEnvironment;
    return std::any_cast<ObjectPtr> (evaluator->visitNode(*ast));
}

} // End of namespace Sysmel
} // End of namespace Compiler
} // End of namespace SysmelMoebius
