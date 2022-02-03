#ifndef SYSMEL_COMPILER_SYSMEL_VISITORS_HPP
#define SYSMEL_COMPILER_SYSMEL_VISITORS_HPP
#pragma once

#include "AST.hpp"

namespace Sysmel
{
namespace Frontend
{
namespace SysmelSyntax
{

struct ASTSequentialVisitor : ASTVisitor
{
    void visitNodeList(ASTNodePtrList &nodeList)
    {
        for(auto &node : nodeList)
            visitNode(*node);
    }

    virtual AnyValuePtr visitParseErrorNode(ASTParseErrorNode &node) override
    {
        (void)node;
        return nullptr;
    }

    virtual AnyValuePtr visitExpressionListNode(ASTExpressionListNode &node) override
    {
        visitNodeList(node.expressions);
        return nullptr;
    }

    virtual AnyValuePtr visitPragmaNode(ASTPragmaNode &node) override
    {
        visitNode(*node.selector);
        visitNodeList(node.arguments);
        return nullptr;
    }

    virtual AnyValuePtr visitBlockNode(ASTBlockNode &node) override
    {
        if(node.blockClosureSignature)
            visitNode(*node.blockClosureSignature);
        visitNodeList(node.pragmas);
        visitNode(*node.expressionList);
        return nullptr;
    }

    virtual AnyValuePtr visitBlockClosureArgumentNode(ASTBlockClosureArgumentNode &node) override
    {
        if(node.type)
            visitNode(*node.type);
        visitNode(*node.identifier);
        return nullptr;
    }
    virtual AnyValuePtr visitBlockClosureSignatureNode(ASTBlockClosureSignatureNode &node) override
    {
        visitNodeList(node.arguments);
        if(node.returnType)
            visitNode(*node.returnType);
        return nullptr;
    }

    virtual AnyValuePtr visitIntegerLiteralNode(ASTIntegerLiteralNode &node) override
    {
        (void)node;
        return nullptr;
    }

    virtual AnyValuePtr visitFloatLiteralNode(ASTFloatLiteralNode &node) override
    {
        (void)node;
        return nullptr;
    }

    virtual AnyValuePtr visitCharacterLiteralNode(ASTCharacterLiteralNode &node) override
    {
        (void)node;
        return nullptr;
    }

    virtual AnyValuePtr visitStringLiteralNode(ASTStringLiteralNode &node) override
    {
        (void)node;
        return nullptr;
    }

    virtual AnyValuePtr visitSymbolLiteralNode(ASTSymbolLiteralNode &node) override
    {
        (void)node;
        return nullptr;
    }

    virtual AnyValuePtr visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node) override
    {
        (void)node;
        return nullptr;
    }

    virtual AnyValuePtr visitMessageSendNode(ASTMessageSendNode &node) override
    {
        visitNode(*node.selector);
        if(node.receiver)
            visitNode(*node.receiver);
        visitNodeList(node.arguments);
        return nullptr;
    }

    virtual AnyValuePtr visitMessageChainNode(ASTMessageChainNode &node) override
    {
        if(node.receiver)
            visitNode(*node.receiver);
        visitNodeList(node.messages);
        return nullptr;
    }

    virtual AnyValuePtr visitMessageChainMessageNode(ASTMessageChainMessageNode &node) override
    {
        visitNode(*node.selector);
        visitNodeList(node.arguments);
        return nullptr;
    }

    virtual AnyValuePtr visitCallNode(ASTCallNode &node) override
    {
        visitNode(*node.callable);
        visitNodeList(node.arguments);
        return nullptr;
    }

    virtual AnyValuePtr visitSubscriptNode(ASTSubscriptNode &node) override
    {
        visitNode(*node.array);
        visitNode(*node.index);
        return nullptr;
    }

    virtual AnyValuePtr visitMakeTupleNode(ASTMakeTupleNode &node) override
    {
        visitNodeList(node.elements);
        return nullptr;
    }

    virtual AnyValuePtr visitMakeDictionaryNode(ASTMakeDictionaryNode &node) override
    {
        visitNodeList(node.elements);
        return nullptr;
    }

    virtual AnyValuePtr visitDictionaryElementNode(ASTDictionaryElementNode &node) override
    {
        visitNode(*node.key);
        if(node.value)
            visitNode(*node.value);
        return nullptr;
    }

    virtual AnyValuePtr visitLiteralArrayNode(ASTLiteralArrayNode &node) override
    {
        visitNodeList(node.elements);
        return nullptr;
    }

    virtual AnyValuePtr visitQuoteNode(ASTQuoteNode &node) override
    {
        visitNode(*node.quoted);
        return nullptr;
    }

    virtual AnyValuePtr visitQuasiQuoteNode(ASTQuasiQuoteNode &node) override
    {
        visitNode(*node.quoted);
        return nullptr;
    }

    virtual AnyValuePtr visitQuasiUnquoteNode(ASTQuasiUnquoteNode &node) override
    {
        visitNode(*node.expression);
        return nullptr;
    }

    virtual AnyValuePtr visitSpliceNode(ASTSpliceNode &node) override
    {
        visitNode(*node.expression);
        return nullptr;
    }
};

template<typename FT>
bool validateASTParseErrors(const ASTNodePtr &ast, const FT &parseErrorCallback)
{
    struct ASTParseErrorValidatingVisitor : ASTSequentialVisitor
    {
        ASTParseErrorValidatingVisitor(const FT &cparseErrorCallback)
            : parseErrorCallback(cparseErrorCallback) {}

        const FT &parseErrorCallback;
        bool hasParseError = false;

        virtual AnyValuePtr visitParseErrorNode(ASTParseErrorNode &node) override
        {
            hasParseError = true;
            parseErrorCallback(node);
            return nullptr;
        }
    };

    ASTParseErrorValidatingVisitor visitor(parseErrorCallback);
    visitor.visitNode(*ast);
    return !visitor.hasParseError;
}

} // End of namespace SysmelSyntax
} // End of namespace Frontend
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_PARSER_AST_HPP
