#ifndef SYSMEL_COMPILER_PARSER_AST_HPP
#define SYSMEL_COMPILER_PARSER_AST_HPP
#pragma once

#include "Token.hpp"
#include "sysmel/Compiler/LiteralInteger.hpp"
#include <memory>
#include <any>

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{

struct ASTNode;
struct ASTParseErrorNode;
struct ASTBlockClosureArgumentNode;
struct ASTBlockClosureSignatureNode;
struct ASTBlockNode;
struct ASTPragmaNode;
struct ASTExpressionListNode;

struct ASTIntegerLiteralNode;
struct ASTFloatLiteralNode;
struct ASTCharacterLiteralNode;
struct ASTStringLiteralNode;
struct ASTSymbolLiteralNode;

struct ASTIdentifierReferenceNode;
struct ASTMessageSendNode;
struct ASTMessageChainNode;
struct ASTMessageChainMessageNode;
struct ASTCallNode;
struct ASTSubscriptNode;
struct ASTMakeTupleNode;
struct ASTMakeDictionaryNode;
struct ASTDictionaryElementNode;
struct ASTLiteralArrayNode;

struct ASTQuoteNode;
struct ASTQuasiQuoteNode;
struct ASTQuasiUnquoteNode;
struct ASTSpliceNode;

struct ASTVisitor;
struct ASTNode : std::enable_shared_from_this<ASTNode>
{
    virtual std::any accept(ASTVisitor &visitor) = 0;

    virtual bool isParseErrorNode() const { return false; }
    virtual bool isExpressionListNode() const { return false; }
    virtual bool isPragmaNode() const { return false; }
    virtual bool isBlockNode() const { return false; }
    virtual bool isBlockClosureArgumentNode() const { return false; }
    virtual bool isBlockClosureSignatureNode() const { return false; }

    virtual bool isIntegerLiteralNode() const { return false; }
    virtual bool isFloatLiteralNode() const { return false; }
    virtual bool isCharacterLiteralNode() const { return false; }
    virtual bool isStringLiteralNode() const { return false; }
    virtual bool isSymbolLiteralNode() const { return false; }

    virtual bool isIdentifierReferenceNode() const { return false; }
    virtual bool isMessageSendNode() const { return false; }
    virtual bool isMessageChainNode() const { return false; }
    virtual bool isMessageChainMessageNode() const { return false; }
    virtual bool isCallNode() const { return false; }
    virtual bool isSubscriptNode() const { return false; }

    virtual bool isMakeTupleNode() const { return false; }
    virtual bool isMakeDictionaryNode() const { return false; }
    virtual bool isDictionaryElementNode() const { return false; }
    virtual bool isLiteralArrayNode() const { return false; }

    virtual bool isQuoteNode() const { return false; }
    virtual bool isQuasiQuoteNode() const { return false; }
    virtual bool isQuasiUnquoteNode() const { return false; }
    virtual bool isSpliceNode() const { return false; }

    template<typename T>
    T &as()
    {
        return static_cast<T&> (*this);
    }

    template<typename T>
    const T &as() const
    {
        return static_cast<const T&> (*this);
    }

    void setTokenRange(const TokenRange &tokenRange)
    {
        tokens = tokenRange;
        sourcePosition = tokenRange.asSourcePosition();
    }

    TokenRange tokens;
    SourcePosition sourcePosition;
};

typedef std::shared_ptr<ASTNode> ASTNodePtr;
typedef std::vector<ASTNodePtr> ASTNodePtrList;

struct ASTVisitor
{
    virtual std::any visitNode(ASTNode &node)
    {
        return node.accept(*this);
    }

    virtual std::any visitParseErrorNode(ASTParseErrorNode &node) = 0;
    virtual std::any visitExpressionListNode(ASTExpressionListNode &node) = 0;
    virtual std::any visitPragmaNode(ASTPragmaNode &node) = 0;
    virtual std::any visitBlockNode(ASTBlockNode &node) = 0;
    virtual std::any visitBlockClosureArgumentNode(ASTBlockClosureArgumentNode &node) = 0;
    virtual std::any visitBlockClosureSignatureNode(ASTBlockClosureSignatureNode &node) = 0;

    virtual std::any visitIntegerLiteralNode(ASTIntegerLiteralNode &node) = 0;
    virtual std::any visitFloatLiteralNode(ASTFloatLiteralNode &node) = 0;
    virtual std::any visitCharacterLiteralNode(ASTCharacterLiteralNode &node) = 0;
    virtual std::any visitStringLiteralNode(ASTStringLiteralNode &node) = 0;
    virtual std::any visitSymbolLiteralNode(ASTSymbolLiteralNode &node) = 0;

    virtual std::any visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node) = 0;
    virtual std::any visitMessageSendNode(ASTMessageSendNode &node) = 0;
    virtual std::any visitMessageChainNode(ASTMessageChainNode &node) = 0;
    virtual std::any visitMessageChainMessageNode(ASTMessageChainMessageNode &node) = 0;
    virtual std::any visitCallNode(ASTCallNode &node) = 0;
    virtual std::any visitSubscriptNode(ASTSubscriptNode &node) = 0;

    virtual std::any visitMakeTupleNode(ASTMakeTupleNode &node) = 0;
    virtual std::any visitMakeDictionaryNode(ASTMakeDictionaryNode &node) = 0;
    virtual std::any visitDictionaryElementNode(ASTDictionaryElementNode &node) = 0;
    virtual std::any visitLiteralArrayNode(ASTLiteralArrayNode &node) = 0;

    virtual std::any visitQuoteNode(ASTQuoteNode &node) = 0;
    virtual std::any visitQuasiQuoteNode(ASTQuasiQuoteNode &node) = 0;
    virtual std::any visitQuasiUnquoteNode(ASTQuasiUnquoteNode &node) = 0;
    virtual std::any visitSpliceNode(ASTSpliceNode &node) = 0;
};

struct ASTExpressionListNode : ASTNode
{
    ASTNodePtrList expressions;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitExpressionListNode(*this);
    }

    virtual bool isExpressionListNode() const override
    {
        return true;
    }
};

struct ASTPragmaNode : ASTNode
{
    ASTNodePtr selector;
    ASTNodePtrList arguments;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitPragmaNode(*this);
    }

    virtual bool isPragmaNode() const override
    {
        return true;
    }
};

struct ASTBlockClosureArgumentNode : ASTNode
{
    ASTNodePtr type;
    ASTNodePtr identifier;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitBlockClosureArgumentNode(*this);
    }

    virtual bool isBlockClosureArgumentNode() const override
    {
        return true;
    }
};
struct ASTBlockClosureSignatureNode : ASTNode
{
    ASTNodePtrList arguments;
    ASTNodePtr returnType;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitBlockClosureSignatureNode(*this);
    }

    virtual bool isBlockClosureSignatureNode() const override
    {
        return true;
    }
};

struct ASTBlockNode : ASTNode
{
    ASTNodePtr blockClosureSignature;
    ASTNodePtrList pragmas;
    ASTNodePtr expressionList;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitBlockNode(*this);
    }

    virtual bool isBlockNode() const override
    {
        return true;
    }

};

struct ASTIntegerLiteralNode : ASTNode
{
    LiteralInteger value;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitIntegerLiteralNode(*this);
    }

    virtual bool isIntegerLiteralNode() const override
    {
        return true;
    }
};

struct ASTFloatLiteralNode : ASTNode
{
    double value;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitFloatLiteralNode(*this);
    }

    virtual bool isFloatLiteralNode() const override
    {
        return true;
    }
};

struct ASTCharacterLiteralNode : ASTNode
{
    uint32_t value;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitCharacterLiteralNode(*this);
    }

    virtual bool isCharacterLiteralNode() const override
    {
        return true;
    }
};

struct ASTStringLiteralNode : ASTNode
{
    std::string value;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitStringLiteralNode(*this);
    }

    virtual bool isStringLiteralNode() const override
    {
        return true;
    }
};

struct ASTSymbolLiteralNode : ASTNode
{
    std::string value;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitSymbolLiteralNode(*this);
    }

    virtual bool isSymbolLiteralNode() const override
    {
        return true;
    }
};

struct ASTParseErrorNode : ASTNode
{
    std::string errorMessage;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitParseErrorNode(*this);
    }

    virtual bool isParseErrorNode() const override
    {
        return true;
    }
};

struct ASTIdentifierReferenceNode : ASTNode
{
    std::string identifier;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitIdentifierReferenceNode(*this);
    }

    virtual bool isIdentifierReferenceNode() const override
    {
        return true;
    }
};

struct ASTMessageSendNode : ASTNode
{
    ASTNodePtr selector;
    ASTNodePtr receiver;
    ASTNodePtrList arguments;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitMessageSendNode(*this);
    }

    virtual bool isMessageSendNode() const override
    {
        return true;
    }
};

struct ASTMessageChainMessageNode : ASTNode
{
    ASTNodePtr selector;
    ASTNodePtrList arguments;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitMessageChainMessageNode(*this);
    }

    virtual bool isMessageChainMessageNode() const override
    {
        return true;
    }
};

struct ASTMessageChainNode : ASTNode
{
    ASTNodePtr receiver;
    ASTNodePtrList messages;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitMessageChainNode(*this);
    }

    virtual bool isMessageChainNode() const override
    {
        return true;
    }
};

struct ASTCallNode : ASTNode
{
    ASTNodePtr callable;
    ASTNodePtrList arguments;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitCallNode(*this);
    }

    virtual bool isCallNode() const override
    {
        return true;
    }
};

struct ASTSubscriptNode : ASTNode
{
    ASTNodePtr array;
    ASTNodePtr index;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitSubscriptNode(*this);
    }

    virtual bool isSubscriptNode() const override
    {
        return true;
    }
};

struct ASTMakeTupleNode : ASTNode
{
    ASTNodePtrList elements;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitMakeTupleNode(*this);
    }

    virtual bool isMakeTupleNode() const override
    {
        return true;
    }
};

struct ASTMakeDictionaryNode : ASTNode
{
    ASTNodePtrList elements;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitMakeDictionaryNode(*this);
    }

    virtual bool isMakeDictionaryNode() const override
    {
        return true;
    }
};

struct ASTDictionaryElementNode : ASTNode
{
    ASTNodePtr key;
    ASTNodePtr value;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitDictionaryElementNode(*this);
    }

    virtual bool isDictionaryElementNode() const override
    {
        return true;
    }
};

struct ASTLiteralArrayNode : ASTNode
{
    ASTNodePtrList elements;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitLiteralArrayNode(*this);
    }

    virtual bool isLiteralArrayNode() const override
    {
        return true;
    }
};

struct ASTQuoteNode : ASTNode
{
    ASTNodePtr quoted;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitQuoteNode(*this);
    }

    virtual bool isQuoteNode() const override
    {
        return true;
    }
};

struct ASTQuasiQuoteNode : ASTNode
{
    ASTNodePtr quoted;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitQuasiQuoteNode(*this);
    }

    virtual bool isQuasiQuoteNode() const override
    {
        return true;
    }
};

struct ASTQuasiUnquoteNode : ASTNode
{
    ASTNodePtr expression;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitQuasiUnquoteNode(*this);
    }

    virtual bool isQuasiUnquoteNode() const override
    {
        return true;
    }
};

struct ASTSpliceNode : ASTNode
{
    ASTNodePtr expression;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitSpliceNode(*this);
    }

    virtual bool isSpliceNode() const override
    {
        return true;
    }
};

inline ASTNodePtr makeParseErrorNodeAt(const TokenRange &tokens, const std::string &errorMessage)
{
    auto node = std::make_shared<ASTParseErrorNode> ();
    node->setTokenRange(tokens);
    node->errorMessage = errorMessage;
    return node;
}

inline ASTNodePtr makeParseErrorNodeAtToken(const TokenRange &position, const std::string &errorMessage)
{
    return makeParseErrorNodeAt(position.until(1), errorMessage);
}

inline ASTNodePtr makeLiteralSymbolASTNodeAt(const TokenRange &tokens, const std::string &value)
{
    auto node = std::make_shared<ASTSymbolLiteralNode> ();
    node->setTokenRange(tokens);
    node->value = value;
    return node;
}

} // End of namespace Sysmel
} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_PARSER_AST_HPP
