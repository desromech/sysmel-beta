#ifndef SYSMEL_BASE_COMPILER_SYSMEL_VISITORS_HPP
#define SYSMEL_BASE_COMPILER_SYSMEL_VISITORS_HPP
#pragma once

#include "AST.hpp"

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{

struct ASTSequentialVisitor : ASTVisitor
{
    void visitNodeList(ASTNodePtrList &nodeList)
    {
        for(auto &node : nodeList)
            visitNode(*node);
    }

    virtual std::any visitParseErrorNode(ASTParseErrorNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitExpressionListNode(ASTExpressionListNode &node) override
    {
        visitNodeList(node.expressions);
        return std::any();
    }

    virtual std::any visitPragmaNode(ASTPragmaNode &node) override
    {
        visitNode(*node.selector);
        visitNodeList(node.arguments);
        return std::any();
    }

    virtual std::any visitBlockNode(ASTBlockNode &node) override
    {
        if(node.blockClosureSignature)
            visitNode(*node.blockClosureSignature);
        visitNodeList(node.pragmas);
        visitNode(*node.expressionList);
        return std::any();
    }

    virtual std::any visitBlockClosureArgumentNode(ASTBlockClosureArgumentNode &node) override
    {
        if(node.type)
            visitNode(*node.type);
        visitNode(*node.identifier);
        return std::any();
    }
    virtual std::any visitBlockClosureSignatureNode(ASTBlockClosureSignatureNode &node) override
    {
        visitNodeList(node.arguments);
        if(node.returnType)
            visitNode(*node.returnType);
        return std::any();
    }

    virtual std::any visitIntegerLiteralNode(ASTIntegerLiteralNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitFloatLiteralNode(ASTFloatLiteralNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitCharacterLiteralNode(ASTCharacterLiteralNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitStringLiteralNode(ASTStringLiteralNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitSymbolLiteralNode(ASTSymbolLiteralNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node) override
    {
        (void)node;
        return std::any();
    }

    virtual std::any visitMessageSendNode(ASTMessageSendNode &node) override
    {
        visitNode(*node.selector);
        if(node.receiver)
            visitNode(*node.receiver);
        visitNodeList(node.arguments);
        return std::any();
    }

    virtual std::any visitMessageChainNode(ASTMessageChainNode &node) override
    {
        if(node.receiver)
            visitNode(*node.receiver);
        visitNodeList(node.messages);
        return std::any();
    }

    virtual std::any visitMessageChainMessageNode(ASTMessageChainMessageNode &node) override
    {
        visitNode(*node.selector);
        visitNodeList(node.arguments);
        return std::any();
    }

    virtual std::any visitCallNode(ASTCallNode &node) override
    {
        visitNode(*node.callable);
        visitNodeList(node.arguments);
        return std::any();
    }

    virtual std::any visitSubscriptNode(ASTSubscriptNode &node) override
    {
        visitNode(*node.array);
        visitNode(*node.index);
        return std::any();
    }

    virtual std::any visitMakeTupleNode(ASTMakeTupleNode &node) override
    {
        visitNodeList(node.elements);
        return std::any();
    }

    virtual std::any visitMakeDictionaryNode(ASTMakeDictionaryNode &node) override
    {
        visitNodeList(node.elements);
        return std::any();
    }

    virtual std::any visitDictionaryElementNode(ASTDictionaryElementNode &node) override
    {
        visitNode(*node.key);
        if(node.value)
            visitNode(*node.value);
        return std::any();
    }

    virtual std::any visitLiteralArrayNode(ASTLiteralArrayNode &node) override
    {
        visitNodeList(node.elements);
        return std::any();
    }

    virtual std::any visitQuoteNode(ASTQuoteNode &node) override
    {
        visitNode(*node.quoted);
        return std::any();
    }

    virtual std::any visitQuasiQuoteNode(ASTQuasiQuoteNode &node) override
    {
        visitNode(*node.quoted);
        return std::any();
    }

    virtual std::any visitQuasiUnquoteNode(ASTQuasiUnquoteNode &node) override
    {
        visitNode(*node.expression);
        return std::any();
    }

    virtual std::any visitSpliceNode(ASTSpliceNode &node) override
    {
        visitNode(*node.expression);
        return std::any();
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

        virtual std::any visitParseErrorNode(ASTParseErrorNode &node) override
        {
            hasParseError = true;
            parseErrorCallback(node);
            return std::any();
        }
    };

    ASTParseErrorValidatingVisitor visitor(parseErrorCallback);
    visitor.visitNode(*ast);
    return !visitor.hasParseError;
}

} // End of namespace Sysmel
} // End of namespace Compiler
} // End of namespace SysmelMoebius

#endif //SYSMEL_BASE_COMPILER_PARSER_AST_HPP
