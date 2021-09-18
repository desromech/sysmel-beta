#include "sysmel/Compiler/Sysmel/SysmelLanguageSupport.hpp"
#include "sysmel/Compiler/Sysmel/Parser.hpp"
#include "sysmel/Compiler/Sysmel/Visitors.hpp"
#include "sysmel/ObjectModel/Error.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"
#include "sysmel/ObjectModel/BootstrapMethod.hpp"

#include "sysmel/ObjectModel/ASTClosureNode.hpp"
#include "sysmel/ObjectModel/ASTLiteralValueNode.hpp"
#include "sysmel/ObjectModel/ASTLexicalScopeNode.hpp"
#include "sysmel/ObjectModel/ASTCleanUpScopeNode.hpp"
#include "sysmel/ObjectModel/ASTIdentifierReferenceNode.hpp"
#include "sysmel/ObjectModel/ASTMessageChainNode.hpp"
#include "sysmel/ObjectModel/ASTMessageChainMessageNode.hpp"
#include "sysmel/ObjectModel/ASTMessageSendNode.hpp"
#include "sysmel/ObjectModel/ASTParseErrorNode.hpp"
#include "sysmel/ObjectModel/ASTSequenceNode.hpp"


#include <fstream>
#include <iostream>

namespace SysmelMoebius
{
namespace Compiler
{
namespace Sysmel
{

using ObjectModel::wrapValue;
using ObjectModel::internSymbol;

class SysmelASTConverter : public ASTVisitor
{
public:
    typedef ObjectModel::ASTNodePtr ResultType;

    ObjectModel::ASTSourcePositionPtr convertSourcePosition(const SourcePosition &originalSourcePosition)
    {
        // TODO: Implement this part.
        return nullptr;
    }

    virtual std::any visitParseErrorNode(ASTParseErrorNode &node) override
    {
        auto convertedNode = std::make_shared<ObjectModel::ASTParseErrorNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->errorMessage = node.errorMessage;
        return ResultType(convertedNode);
    }

    virtual std::any visitExpressionListNode(ASTExpressionListNode &node) override
    {
        auto convertedNode = std::make_shared<ObjectModel::ASTSequenceNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expressions.reserve(node.expressions.size());
        for(auto expr : node.expressions)
        {
            convertedNode->expressions.push_back(std::any_cast<ResultType> (visitNode(*expr)));
        }

        return ResultType(convertedNode);
    }
    
    virtual std::any visitPragmaNode(ASTPragmaNode &) override
    {
        return std::any();
    }
    
    virtual std::any visitBlockNode(ASTBlockNode &node) override
    {
        auto convertedSequence = std::any_cast<ResultType> (visitNode(*node.expressionList));

        auto blockPosition = convertSourcePosition(node.sourcePosition);

        if(node.blockClosureSignature)
        {
            auto signature = std::static_pointer_cast<ASTBlockClosureSignatureNode> (node.blockClosureSignature);
            auto blockClosureNode = std::make_shared<ObjectModel::ASTClosureNode> ();
            blockClosureNode->kind = ObjectModel::ASTClosureNodeKind::Block;

            return std::any();
        }
        else
        {
            auto lexicalScope = std::make_shared<ObjectModel::ASTLexicalScopeNode> ();
            lexicalScope->sourcePosition = blockPosition;
            lexicalScope->body = convertedSequence;

            auto cleanUpScope = std::make_shared<ObjectModel::ASTCleanUpScopeNode> ();
            cleanUpScope->sourcePosition = blockPosition;
            cleanUpScope->body = lexicalScope;
            return ResultType(cleanUpScope);
        }
    }
    
    virtual std::any visitBlockClosureArgumentNode(ASTBlockClosureArgumentNode &)
    {
        return std::any();
    }
    
    virtual std::any visitBlockClosureSignatureNode(ASTBlockClosureSignatureNode &)
    {
        // This should be unreachable.
        return std::any();
    }

    virtual std::any visitIntegerLiteralNode(ASTIntegerLiteralNode &node) override
    {
        auto convertedNode = std::make_shared<ObjectModel::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitFloatLiteralNode(ASTFloatLiteralNode &node) override
    {
        auto convertedNode = std::make_shared<ObjectModel::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitCharacterLiteralNode(ASTCharacterLiteralNode &node) override
    {
        auto convertedNode = std::make_shared<ObjectModel::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitStringLiteralNode(ASTStringLiteralNode &node) override
    {
        auto convertedNode = std::make_shared<ObjectModel::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitSymbolLiteralNode(ASTSymbolLiteralNode &node) override
    {
        auto convertedNode = std::make_shared<ObjectModel::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(internSymbol(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node) override
    {
        auto convertedNode = std::make_shared<ObjectModel::ASTIdentifierReferenceNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->identifier = internSymbol(node.identifier);
        return ResultType(convertedNode);
    }

    virtual std::any visitMessageSendNode(ASTMessageSendNode &node) override
    {
        auto convertedNode = std::make_shared<ObjectModel::ASTMessageSendNode> ();
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
        auto convertedNode = std::make_shared<ObjectModel::ASTMessageChainNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);

        if(node.receiver)
            convertedNode->receiver = std::any_cast<ResultType> (visitNode(*node.receiver));

        convertedNode->messages.reserve(node.messages.size());
        for(auto &message : node.messages)
            convertedNode->messages.push_back(std::any_cast<ObjectModel::ASTMessageChainMessageNodePtr> (visitNode(*message)));

        return ResultType(convertedNode);
    }

    virtual std::any visitMessageChainMessageNode(ASTMessageChainMessageNode &node) override
    {
        auto convertedNode = std::make_shared<ObjectModel::ASTMessageChainMessageNode> ();
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

    virtual std::any visitQuoteNode(ASTQuoteNode &) override
    {
        return std::any();
    }

    virtual std::any visitQuasiQuoteNode(ASTQuasiQuoteNode &) override
    {
        return std::any();
    }

    virtual std::any visitQuasiUnquoteNode(ASTQuasiUnquoteNode &) override
    {
        return std::any();
    }

    virtual std::any visitSpliceNode(ASTSpliceNode &) override
    {
        return std::any();
    }
};

} // End of namespace Sysmel
} // End of namespace Compiler


namespace ObjectModel
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

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius
