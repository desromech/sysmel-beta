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
struct ASTExpressionListNode;

struct ASTIntegerLiteralNode;
struct ASTFloatLiteralNode;
struct ASTCharacterLiteralNode;
struct ASTStringLiteralNode;
struct ASTSymbolLiteralNode;

struct ASTVisitor;
struct ASTNode : std::enable_shared_from_this<ASTNode>
{
    virtual std::any accept(ASTVisitor &visitor) = 0;

    virtual bool isParseErrorNode() const { return false; }
    virtual bool isExpressionListNode() const { return false; }

    virtual bool isIntegerLiteralNode() const { return false; }
    virtual bool isFloatLiteralNode() const { return false; }
    virtual bool isCharacterLiteralNode() const { return false; }
    virtual bool isStringLiteralNode() const { return false; }
    virtual bool isSymbolLiteralNode() const { return false; }

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

struct ASTVisitor
{
    virtual std::any visitNode(ASTNode &node)
    {
        return node.accept(*this);
    }

    virtual std::any visitParseErrorNode(ASTParseErrorNode &node) = 0;
    virtual std::any visitExpressionListNode(ASTExpressionListNode &node) = 0;
    virtual std::any visitIntegerLiteralNode(ASTIntegerLiteralNode &node) = 0;
    virtual std::any visitFloatLiteralNode(ASTFloatLiteralNode &node) = 0;
    virtual std::any visitCharacterLiteralNode(ASTCharacterLiteralNode &node) = 0;
    virtual std::any visitStringLiteralNode(ASTStringLiteralNode &node) = 0;
    virtual std::any visitSymbolLiteralNode(ASTSymbolLiteralNode &node) = 0;
};

struct ASTExpressionListNode : ASTNode
{
    std::vector<ASTNodePtr> expressions;

    virtual std::any accept(ASTVisitor &visitor) override
    {
        return visitor.visitExpressionListNode(*this);
    }

    virtual bool isExpressionListNode() const override
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

inline ASTNodePtr makeParseErrorNodeAt(const TokenRange &tokens, const std::string &errorMessage)
{
    auto node = std::make_shared<ASTParseErrorNode> ();
    node->setTokenRange(tokens);
    node->errorMessage = errorMessage;
    return node;
}

} // End of namespace Sysmel
} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_PARSER_AST_HPP
