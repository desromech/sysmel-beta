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
#include "sysmel/BootstrapEnvironment/ASTMakeAssociationNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeDictionaryNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeLiteralArrayNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeTupleNode.hpp"
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

#include "sysmel/BootstrapEnvironment/ASTSourceCode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSourceCodePosition.hpp"

#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"

#include "sysmel/BootstrapEnvironment/IdentifierLookupScope.hpp"
#include "sysmel/BootstrapEnvironment/LexicalScope.hpp"

#include "sysmel/BootstrapEnvironment/MetaBuilderFactory.hpp"
#include "sysmel/BootstrapEnvironment/LetMetaBuilder.hpp"

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

    std::unordered_map<SourceCollectionPtr, BootstrapEnvironment::ASTSourceCodePtr> convertedSourceCollectionMap;
    std::unordered_map<SourcePosition, BootstrapEnvironment::ASTSourcePositionPtr> convertedSourcePositionMap;


    BootstrapEnvironment::ASTSourceCodePtr convertSourceCollection(const SourceCollectionPtr &originalSourceCollection)
    {
        auto it = convertedSourceCollectionMap.find(originalSourceCollection);
        if(it != convertedSourceCollectionMap.end())
            return it->second;

        auto result = std::make_shared<BootstrapEnvironment::ASTSourceCode> ();
        result->name = originalSourceCollection->name;
        result->contents = originalSourceCollection->text;
        convertedSourceCollectionMap.insert(std::make_pair(originalSourceCollection, result));
        return result;
    }

    BootstrapEnvironment::ASTSourcePositionPtr convertSourcePosition(const SourcePosition &originalSourcePosition)
    {
        if(!originalSourcePosition.collection)
            return BootstrapEnvironment::ASTSourcePosition::empty();

        auto it = convertedSourcePositionMap.find(originalSourcePosition);
        if(it != convertedSourcePositionMap.end())
            return it->second;

        auto result = std::make_shared<BootstrapEnvironment::ASTSourceCodePosition> ();
        result->sourceCode = convertSourceCollection(originalSourcePosition.collection);
        result->startPosition = originalSourcePosition.startPosition;

        auto startLineColumn = originalSourcePosition.startLineColumn();
        result->startLine = startLineColumn.line;
        result->startColumn = startLineColumn.column;

        result->endPosition = originalSourcePosition.endPosition;

        auto endLineColumn = originalSourcePosition.endLineColumn();
        result->endLine = endLineColumn.line;
        result->endColumn = endLineColumn.column;

        convertedSourcePositionMap.insert(std::make_pair(originalSourcePosition, result));
        return result;
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

    virtual std::any visitCallNode(ASTCallNode &node) override
    {
        auto sourcePosition = convertSourcePosition(node.sourcePosition);

        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTMessageSendNode> ();
        convertedNode->sourcePosition = sourcePosition;

        auto selector = std::make_shared<BootstrapEnvironment::ASTLiteralValueNode>  ();
        selector->sourcePosition = sourcePosition;
        selector->setValueAndType(internSymbol("()"));

        auto tuple = std::make_shared<BootstrapEnvironment::ASTMakeTupleNode>  ();
        tuple->sourcePosition = sourcePosition;
        tuple->elements.reserve(node.arguments.size());
        for(auto &arg : node.arguments)
            tuple->elements.push_back(std::any_cast<ResultType> (visitNode(*arg)));

        convertedNode->receiver = std::any_cast<ResultType> (visitNode(*node.callable));
        convertedNode->selector = selector;

        convertedNode->arguments.reserve(1);
        convertedNode->arguments.push_back(tuple);

        return ResultType(convertedNode);
    }

    virtual std::any visitSubscriptNode(ASTSubscriptNode &node) override
    {
        auto sourcePosition = convertSourcePosition(node.sourcePosition);

        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTMessageSendNode> ();
        convertedNode->sourcePosition = sourcePosition;

        auto selector = std::make_shared<BootstrapEnvironment::ASTLiteralValueNode>  ();
        selector->sourcePosition = sourcePosition;
        selector->setValueAndType(internSymbol("[]"));

        convertedNode->receiver = std::any_cast<ResultType> (visitNode(*node.array));
        convertedNode->selector = selector;

        convertedNode->arguments.reserve(1);
        convertedNode->arguments.push_back(std::any_cast<ResultType> (visitNode(*node.index)));

        return ResultType(convertedNode);
    }

    virtual std::any visitMakeTupleNode(ASTMakeTupleNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTMakeTupleNode>  ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->elements.reserve(node.elements.size());
        for(auto &element : node.elements)
            convertedNode->elements.push_back(std::any_cast<ResultType> (visitNode(*element)));

        return ResultType(convertedNode);
    }

    virtual std::any visitMakeDictionaryNode(ASTMakeDictionaryNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTMakeDictionaryNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->elements.reserve(node.elements.size());
        for(const auto &element : node.elements)
            convertedNode->elements.push_back(std::any_cast<ResultType> (visitNode(*element)));

        return ResultType(convertedNode);
    }

    virtual std::any visitDictionaryElementNode(ASTDictionaryElementNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTMakeAssociationNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        if(node.key)
            convertedNode->key = std::any_cast<ResultType> (visitNode(*node.key));
        if(node.value)
            convertedNode->value = std::any_cast<ResultType> (visitNode(*node.value));

        return ResultType(convertedNode);
    }

    virtual std::any visitLiteralArrayNode(ASTLiteralArrayNode &node) override
    {
        auto convertedNode = std::make_shared<BootstrapEnvironment::ASTMakeLiteralArrayNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->elements.reserve(node.elements.size());
        for(const auto &element : node.elements)
            convertedNode->elements.push_back(std::any_cast<ResultType> (visitNode(*element)));

        return ResultType(convertedNode);
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
    return std::static_pointer_cast<SysmelLanguageSupport> (RuntimeContext::getActive()->getSysmelLanguageSupport());
}

void SysmelLanguageSupport::initialize()
{
    SuperType::initialize();

    {
        keywordScope = std::make_shared<LexicalScope> ();
        keywordScope->setSymbolBinding(internSymbol("true"), getTrueConstant());
        keywordScope->setSymbolBinding(internSymbol("false"), getFalseConstant());
        keywordScope->setSymbolBinding(internSymbol("nil"), getNilConstant());
        keywordScope->setSymbolBinding(internSymbol("void"), getVoidConstant());
    }

    {
        topLevelScope = std::make_shared<LexicalScope> ();
        topLevelScope->parent = keywordScope;

        // Meta builders
        topLevelScope->setSymbolBinding(internSymbol("let"), metaBuilderFactoryFor<LetMetaBuilder> ("let"));
    }
}

LexicalScopePtr SysmelLanguageSupport::createDefaultTopLevelLexicalScope()
{
    return topLevelScope;
}

LexicalScopePtr SysmelLanguageSupport::createMakeLiteralArrayTopLevelLexicalScope()
{
    return keywordScope;
}

ASTNodePtr SysmelLanguageSupport::parseSourceStringNamed(const std::string &sourceString, const std::string &sourceStringName)
{
    auto sysmelAST = SysmelMoebius::Compiler::Sysmel::parseString(sourceString, sourceStringName);
    return std::any_cast<ASTNodePtr> (SysmelMoebius::Compiler::Sysmel::SysmelASTConverter().visitNode(*sysmelAST));
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius
