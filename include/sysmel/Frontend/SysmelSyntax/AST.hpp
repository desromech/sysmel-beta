#ifndef SYSMEL_COMPILER_PARSER_AST_HPP
#define SYSMEL_COMPILER_PARSER_AST_HPP
#pragma once

#include "Token.hpp"
#include "Environment/LargeInteger.hpp"
#include "Environment/AnyValue.hpp"
#include <memory>

namespace Sysmel
{
namespace Frontend
{
namespace SysmelSyntax
{

using Sysmel::Environment::LargeInteger;
using Sysmel::Environment::AnyValuePtr;

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
struct ASTNode
{
    virtual ~ASTNode() {}
    
    virtual AnyValuePtr accept(ASTVisitor &visitor) = 0;

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
    virtual AnyValuePtr visitNode(ASTNode &node)
    {
        return node.accept(*this);
    }

    virtual AnyValuePtr visitParseErrorNode(ASTParseErrorNode &node) = 0;
    virtual AnyValuePtr visitExpressionListNode(ASTExpressionListNode &node) = 0;
    virtual AnyValuePtr visitPragmaNode(ASTPragmaNode &node) = 0;
    virtual AnyValuePtr visitBlockNode(ASTBlockNode &node) = 0;
    virtual AnyValuePtr visitBlockClosureArgumentNode(ASTBlockClosureArgumentNode &node) = 0;
    virtual AnyValuePtr visitBlockClosureSignatureNode(ASTBlockClosureSignatureNode &node) = 0;

    virtual AnyValuePtr visitIntegerLiteralNode(ASTIntegerLiteralNode &node) = 0;
    virtual AnyValuePtr visitFloatLiteralNode(ASTFloatLiteralNode &node) = 0;
    virtual AnyValuePtr visitCharacterLiteralNode(ASTCharacterLiteralNode &node) = 0;
    virtual AnyValuePtr visitStringLiteralNode(ASTStringLiteralNode &node) = 0;
    virtual AnyValuePtr visitSymbolLiteralNode(ASTSymbolLiteralNode &node) = 0;

    virtual AnyValuePtr visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node) = 0;
    virtual AnyValuePtr visitMessageSendNode(ASTMessageSendNode &node) = 0;
    virtual AnyValuePtr visitMessageChainNode(ASTMessageChainNode &node) = 0;
    virtual AnyValuePtr visitMessageChainMessageNode(ASTMessageChainMessageNode &node) = 0;
    virtual AnyValuePtr visitCallNode(ASTCallNode &node) = 0;
    virtual AnyValuePtr visitSubscriptNode(ASTSubscriptNode &node) = 0;

    virtual AnyValuePtr visitMakeTupleNode(ASTMakeTupleNode &node) = 0;
    virtual AnyValuePtr visitMakeDictionaryNode(ASTMakeDictionaryNode &node) = 0;
    virtual AnyValuePtr visitDictionaryElementNode(ASTDictionaryElementNode &node) = 0;
    virtual AnyValuePtr visitLiteralArrayNode(ASTLiteralArrayNode &node) = 0;

    virtual AnyValuePtr visitQuoteNode(ASTQuoteNode &node) = 0;
    virtual AnyValuePtr visitQuasiQuoteNode(ASTQuasiQuoteNode &node) = 0;
    virtual AnyValuePtr visitQuasiUnquoteNode(ASTQuasiUnquoteNode &node) = 0;
    virtual AnyValuePtr visitSpliceNode(ASTSpliceNode &node) = 0;
};

struct ASTExpressionListNode : ASTNode
{
    ASTNodePtrList expressions;

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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
    LargeInteger value;

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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
    char32_t value;

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

    virtual AnyValuePtr accept(ASTVisitor &visitor) override
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

} // End of namespace SysmelSyntax
} // End of namespace Frontend
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_PARSER_AST_HPP
