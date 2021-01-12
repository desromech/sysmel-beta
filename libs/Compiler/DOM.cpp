#include "sysmel/Compiler/DOM.hpp"
#include "sysmel/Compiler/Sysmel/Parser.hpp"
#include "sysmel/Compiler/Sysmel/Visitors.hpp"
#include <type_traits>

namespace SysmelMoebius
{
namespace Compiler
{
namespace DOM
{

using namespace Sysmel;

struct SysmelASTLiteralToDOMVisitor : ASTVisitor
{
    // Unused nodes.
    virtual std::any visitParseErrorNode(ASTParseErrorNode &) override { abort(); }
    virtual std::any visitExpressionListNode(ASTExpressionListNode &) override { abort(); }
    virtual std::any visitPragmaNode(ASTPragmaNode &) override { abort(); }
    virtual std::any visitBlockNode(ASTBlockNode &) override { abort(); }
    virtual std::any visitBlockClosureArgumentNode(ASTBlockClosureArgumentNode &) override { abort(); }
    virtual std::any visitBlockClosureSignatureNode(ASTBlockClosureSignatureNode &) override { abort(); }

    virtual std::any visitMessageSendNode(ASTMessageSendNode &) override { abort(); }
    virtual std::any visitMessageChainNode(ASTMessageChainNode &) override { abort(); }
    virtual std::any visitMessageChainMessageNode(ASTMessageChainMessageNode &) override { abort(); }
    virtual std::any visitCallNode(ASTCallNode &) override { abort(); }
    virtual std::any visitSubscriptNode(ASTSubscriptNode &) override { abort(); }

    virtual std::any visitMakeTupleNode(ASTMakeTupleNode &) override { abort(); }

    virtual std::any visitQuoteNode(ASTQuoteNode &) override { abort(); }
    virtual std::any visitQuasiQuoteNode(ASTQuasiQuoteNode &) override { abort(); }
    virtual std::any visitQuasiUnquoteNode(ASTQuasiUnquoteNode &) override { abort(); }
    virtual std::any visitSpliceNode(ASTSpliceNode &) override { abort(); }

    // DOM Atom literals
    virtual std::any visitIntegerLiteralNode(ASTIntegerLiteralNode &node) override
    {
        return Value(node.value);
    }

    virtual std::any visitFloatLiteralNode(ASTFloatLiteralNode &node) override
    {
        return Value(node.value);
    }

    virtual std::any visitCharacterLiteralNode(ASTCharacterLiteralNode &node) override
    {
        return Value(Character{node.value});
    }

    virtual std::any visitStringLiteralNode(ASTStringLiteralNode &node) override
    {
        return Value(String{node.value});
    }

    virtual std::any visitSymbolLiteralNode(ASTSymbolLiteralNode &node) override
    {
        return Value(Symbol{node.value});
    }

    virtual std::any visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node) override
    {
        if(node.identifier == "nil")
            return Value(Nil());
        else if(node.identifier == "true")
            return Value(True());
        else if(node.identifier == "false")
            return Value(False());
        else if(node.identifier == "void")
            return Value(Void());

        return Value(Symbol{node.identifier});
    }

    // DOM Composite nodes
    virtual std::any visitLiteralArrayNode(ASTLiteralArrayNode &node) override
    {
        Array array;
        for(auto &e : node.elements)
        {
            array.push_back(std::any_cast<Value> (visitNode(*e)));
        }

        return Value(array);
    }

    virtual std::any visitMakeDictionaryNode(ASTMakeDictionaryNode &node) override
    {
        Dictionary dictionary;

        for(auto &e : node.elements)
        {
            dictionary.insert(std::any_cast<Dictionary::value_type> (visitNode(*e)));
        }

        return Value(dictionary);
    }

    virtual std::any visitDictionaryElementNode(ASTDictionaryElementNode &node) override
    {
        auto key = std::any_cast<Value> (visitNode(*node.key));
        Value value = Void();
        if(node.value)
            value = std::any_cast<Value> (visitNode(*node.value));

        auto castedKey = std::visit([](const auto &element) {
            typedef decltype(element) ElementType;
            if constexpr(std::is_same<ElementType, const Array &> () ||
                std::is_same<ElementType, const Dictionary &> ())
                return Key(Void());
            else
                return Key(element);
        }, key);

        return Dictionary::value_type(castedKey, value);
    }
};

ParseResult parseWithSysmelSyntax(const SourceCollectionPtr &source)
{
    auto ast = parseTokenListWithLiteralArrayContent(scanSourceCollection(source));

    bool hasSetParseError = false;
    ParseError parseError;

    // Validate the parse errors.
    if(!validateASTParseErrors(ast, [&](const ASTParseErrorNode &parseErrorNode) {
        if(hasSetParseError)
            return;

        hasSetParseError = true;
        parseError.location = parseErrorNode.sourcePosition;
        parseError.errorMessage = parseErrorNode.errorMessage;
    })) {
        return parseError;
    }

    // Construct the DOM value.
    auto value = std::any_cast<Value> (SysmelASTLiteralToDOMVisitor().visitNode(*ast));
    return value;
}

} // End of namespace DOM
} // End of namespace Compiler
} // End of namespace SysmelMoebius
