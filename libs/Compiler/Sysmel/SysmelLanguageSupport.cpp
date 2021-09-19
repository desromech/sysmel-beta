#include "sysmel/Compiler/Sysmel/SysmelLanguageSupport.hpp"
#include "sysmel/Compiler/Sysmel/Parser.hpp"
#include "sysmel/Compiler/Sysmel/Visitors.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"

#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTClosureNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLexicalScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCleanUpScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTIdentifierReferenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainMessageNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTPragmaNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuasiQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuasiUnquoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSpliceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTParseErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSequenceNode.hpp"


#include <fstream>
#include <iostream>

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{

using BootstrapEnvironment::wrapValue;
using BootstrapEnvironment::internSymbol;

class SysmelASTConverter : public ASTVisitor
{
public:
    typedef BootstrapEnvironment::ASTNodePtr ResultType;

    BootstrapEnvironment::ASTSourcePositionPtr convertSourcePosition(const SourcePosition &originalSourcePosition)
    {
        // TODO: Implement this part.
        return nullptr;
    }

    virtual std::any visitParseErrorNode(ASTParseErrorNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTParseErrorNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->errorMessage = node.errorMessage;
        return ResultType(convertedNode);
    }

    virtual std::any visitExpressionListNode(ASTExpressionListNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTSequenceNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expressions.reserve(node.expressions.size());
        for(auto expr : node.expressions)
        {
            convertedNode->expressions.push_back(std::any_cast<ResultType> (visitNode(*expr)));
        }

        return ResultType(convertedNode);
    }
    
    virtual std::any visitPragmaNode(ASTPragmaNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTPragmaNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);

        convertedNode->selector = std::any_cast<ResultType> (visitNode(*node.selector));
        convertedNode->arguments.reserve(node.arguments.size());
        for(auto &arg : node.arguments)
            convertedNode->arguments.push_back(std::any_cast<ResultType> (visitNode(*arg)));

        return ResultType(convertedNode);
    }
    
    virtual std::any visitBlockNode(ASTBlockNode &node) override
    {
        auto convertedSequence = std::static_pointer_cast<BootstrapEnvironment::ASTSequenceNode> (std::any_cast<ResultType> (visitNode(*node.expressionList)));
        auto blockPosition = convertSourcePosition(node.sourcePosition);

        convertedSequence->pragmas.reserve(node.pragmas.size());
        for(const auto &pragma : node.pragmas)
            convertedSequence->pragmas.push_back(std::any_cast<ResultType> (visitNode(*pragma)));

        if(node.blockClosureSignature)
        {
            auto signature = std::static_pointer_cast<ASTBlockClosureSignatureNode> (node.blockClosureSignature);
            auto blockClosureNode = std::make_shared<BootstrapEnvironment::ASTClosureNode> ();
            blockClosureNode->kind = BootstrapEnvironment::ASTClosureNodeKind::Block;
            blockClosureNode->arguments.reserve(signature->arguments.size());
            for(const auto &arg : signature->arguments)
                blockClosureNode->arguments.push_back(std::any_cast<ResultType> (visitNode(*arg)));

            if(signature->returnType)
                blockClosureNode->returnType = std::any_cast<ResultType> (visitNode(*signature->returnType));
            blockClosureNode->body = convertedSequence;
            
            return ResultType(blockClosureNode);
        }
        else
        {
            auto lexicalScope = std::make_shared<BootstrapEnvironment::ASTLexicalScopeNode> ();
            lexicalScope->sourcePosition = blockPosition;
            lexicalScope->body = convertedSequence;

            auto cleanUpScope = std::make_shared<BootstrapEnvironment::ASTCleanUpScopeNode> ();
            cleanUpScope->sourcePosition = blockPosition;
            cleanUpScope->body = lexicalScope;
            return ResultType(cleanUpScope);
        }
    }
    
    virtual std::any visitBlockClosureArgumentNode(ASTBlockClosureArgumentNode &node)
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTArgumentDefinitionNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        if(node.identifier)
            convertedNode->identifier = std::any_cast<ResultType> (visitNode(*node.identifier));
        if(node.type)
            convertedNode->type = std::any_cast<ResultType> (visitNode(*node.type));
        return ResultType(convertedNode);
    }
    
    virtual std::any visitBlockClosureSignatureNode(ASTBlockClosureSignatureNode &)
    {
        // This should be unreachable.
        return std::any();
    }

    virtual std::any visitIntegerLiteralNode(ASTIntegerLiteralNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitFloatLiteralNode(ASTFloatLiteralNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitCharacterLiteralNode(ASTCharacterLiteralNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitStringLiteralNode(ASTStringLiteralNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitSymbolLiteralNode(ASTSymbolLiteralNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(internSymbol(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTIdentifierReferenceNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->identifier = internSymbol(node.identifier);
        return ResultType(convertedNode);
    }

    virtual std::any visitMessageSendNode(ASTMessageSendNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTMessageSendNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);

        if(node.receiver)
            convertedNode->receiver = std::any_cast<ResultType> (visitNode(*node.receiver));

        convertedNode->selector = std::any_cast<ResultType> (visitNode(*node.selector));

        convertedNode->arguments.reserve(node.arguments.size());
        for(auto &arg : node.arguments)
            convertedNode->arguments.push_back(std::any_cast<ResultType> (visitNode(*arg)));

        return ResultType(convertedNode);
    }

    virtual std::any visitMessageChainNode(ASTMessageChainNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTMessageChainNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);

        if(node.receiver)
            convertedNode->receiver = std::any_cast<ResultType> (visitNode(*node.receiver));

        convertedNode->messages.reserve(node.messages.size());
        for(auto &message : node.messages)
            convertedNode->messages.push_back(std::any_cast<BootstrapEnvironment::ASTMessageChainMessageNodePtr> (visitNode(*message)));

        return ResultType(convertedNode);
    }

    virtual std::any visitMessageChainMessageNode(ASTMessageChainMessageNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTMessageChainMessageNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);

        convertedNode->selector = std::any_cast<ResultType> (visitNode(*node.selector));

        convertedNode->arguments.reserve(node.arguments.size());
        for(auto &arg : node.arguments)
            convertedNode->arguments.push_back(std::any_cast<ResultType> (visitNode(*arg)));

        return convertedNode;
    }

    virtual std::any visitCallNode(ASTCallNode &) override
    {
        return std::any();
    }

    virtual std::any visitSubscriptNode(ASTSubscriptNode &) override
    {
        return std::any();
    }

    virtual std::any visitMakeTupleNode(ASTMakeTupleNode &) override
    {
        return std::any();
    }

    virtual std::any visitMakeDictionaryNode(ASTMakeDictionaryNode &) override
    {
        return std::any();
    }

    virtual std::any visitDictionaryElementNode(ASTDictionaryElementNode &) override
    {
        return std::any();
    }

    virtual std::any visitLiteralArrayNode(ASTLiteralArrayNode &) override
    {
        return std::any();
    }

    virtual std::any visitQuoteNode(ASTQuoteNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTQuoteNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expression = std::any_cast<ResultType> (visitNode(*node.quoted));
        return ResultType(convertedNode);
    }

    virtual std::any visitQuasiQuoteNode(ASTQuasiQuoteNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTQuasiQuoteNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expression = std::any_cast<ResultType> (visitNode(*node.quoted));
        return ResultType(convertedNode);
    }

    virtual std::any visitQuasiUnquoteNode(ASTQuasiUnquoteNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTQuasiUnquoteNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expression = std::any_cast<ResultType> (visitNode(*node.expression));
        return ResultType(convertedNode);
    }

    virtual std::any visitSpliceNode(ASTSpliceNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTSpliceNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expression = std::any_cast<ResultType> (visitNode(*node.expression));
        return ResultType(convertedNode);
    }
};

} // End of namespace Sysmel
} // End of namespace Compiler


namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<SysmelLanguageSupport> sysmelLanguageSupportTypeRegistration;

MethodCategories SysmelLanguageSupport::__typeMethods__()
{
    return MethodCategories{
        {"singleton", {
            makeMethodBinding<SysmelLanguageSupportPtr (AnyValuePtr)> ("uniqueInstance", [](const AnyValuePtr &) {
                return SysmelLanguageSupport::uniqueInstance();
            }),
        }},
    };
}

SysmelLanguageSupportPtr SysmelLanguageSupport::uniqueInstance()
{
    static SysmelLanguageSupportPtr singleton = std::make_shared<SysmelLanguageSupport> ();
    return singleton;
}

ASTNodePtr SysmelLanguageSupport::parseSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName) const
{
    auto sysmelAST = SysmelMoebius::Compiler::Sysmel::parseString(sourceString, sourceStringName);
    return std::any_cast<ASTNodePtr> (SysmelMoebius::Compiler::Sysmel::SysmelASTConverter().visitNode(*sysmelAST));
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius
