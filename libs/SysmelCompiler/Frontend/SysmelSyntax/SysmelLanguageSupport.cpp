#include "Frontend/SysmelSyntax/SysmelLanguageSupport.hpp"
#include "Frontend/SysmelSyntax/Parser.hpp"
#include "Frontend/SysmelSyntax/Visitors.hpp"
#include "Environment/Error.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"

#include "Environment/ASTArgumentDefinitionNode.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTClosureNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTLexicalScopeNode.hpp"
#include "Environment/ASTCleanUpScopeNode.hpp"
#include "Environment/ASTIdentifierReferenceNode.hpp"
#include "Environment/ASTMakeAssociationNode.hpp"
#include "Environment/ASTMakeDictionaryNode.hpp"
#include "Environment/ASTMakeLiteralArrayNode.hpp"
#include "Environment/ASTMakeTupleNode.hpp"
#include "Environment/ASTMessageChainNode.hpp"
#include "Environment/ASTMessageChainMessageNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTPragmaNode.hpp"
#include "Environment/ASTQuoteNode.hpp"
#include "Environment/ASTQuasiQuoteNode.hpp"
#include "Environment/ASTQuasiUnquoteNode.hpp"
#include "Environment/ASTSpliceNode.hpp"
#include "Environment/ASTParseErrorNode.hpp"
#include "Environment/ASTSequenceNode.hpp"

#include "Environment/ASTSourceCode.hpp"
#include "Environment/ASTSourceCodePosition.hpp"

#include "Environment/RuntimeContext.hpp"
#include "Environment/BootstrapModule.hpp"
#include "Environment/Namespace.hpp"

#include "Environment/IdentifierLookupScope.hpp"
#include "Environment/LexicalScope.hpp"

#include "Environment/MetaBuilderFactory.hpp"
#include "Environment/FunctionMetaBuilder.hpp"
#include "Environment/LetMetaBuilder.hpp"
#include "Environment/NamespaceMetaBuilder.hpp"

#include "Environment/EnumMetaBuilder.hpp"
#include "Environment/StructMetaBuilder.hpp"
#include "Environment/UnionMetaBuilder.hpp"
#include "Environment/ClassMetaBuilder.hpp"

#include "Environment/InternalMetaBuilder.hpp"
#include "Environment/PrivateMetaBuilder.hpp"
#include "Environment/ProtectedMetaBuilder.hpp"
#include "Environment/PublicMetaBuilder.hpp"

#include "Environment/FieldMetaBuilder.hpp"
#include "Environment/GlobalMetaBuilder.hpp"

#include "Environment/CompileTimeMetaBuilder.hpp"

#include "Environment/ASTBuilder.hpp"
#include "Environment/ASTIfNode.hpp"
#include "Environment/ASTWhileNode.hpp"
#include "Environment/ASTDoWhileNode.hpp"
#include "Environment/ASTReturnNode.hpp"
#include "Environment/ASTBreakNode.hpp"
#include "Environment/ASTContinueNode.hpp"

#include <fstream>

namespace Sysmel
{
namespace Frontend
{
namespace SysmelSyntax
{

using Environment::wrapValue;
using Environment::internSymbol;

class SysmelASTConverter : public ASTVisitor
{
public:
    typedef Environment::ASTNodePtr ResultType;

    std::unordered_map<SourceCollectionPtr, Environment::ASTSourceCodePtr> convertedSourceCollectionMap;
    std::unordered_map<SourcePosition, Environment::ASTSourcePositionPtr> convertedSourcePositionMap;


    Environment::ASTSourceCodePtr convertSourceCollection(const SourceCollectionPtr &originalSourceCollection)
    {
        auto it = convertedSourceCollectionMap.find(originalSourceCollection);
        if(it != convertedSourceCollectionMap.end())
            return it->second;

        auto result = Environment::basicMakeObject<Environment::ASTSourceCode> ();
        result->name = originalSourceCollection->name;
        result->contents = originalSourceCollection->text;
        convertedSourceCollectionMap.insert(std::make_pair(originalSourceCollection, result));
        return result;
    }

    Environment::ASTSourcePositionPtr convertSourcePosition(const SourcePosition &originalSourcePosition)
    {
        if(!originalSourcePosition.collection)
            return Environment::ASTSourcePosition::empty();

        auto it = convertedSourcePositionMap.find(originalSourcePosition);
        if(it != convertedSourcePositionMap.end())
            return it->second;

        auto result = Environment::basicMakeObject<Environment::ASTSourceCodePosition> ();
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
        auto convertedNode = Environment::basicMakeObject<Environment::ASTParseErrorNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->errorMessage = node.errorMessage;
        return ResultType(convertedNode);
    }

    virtual std::any visitExpressionListNode(ASTExpressionListNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTSequenceNode> ();
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
        auto convertedNode = Environment::basicMakeObject<Environment::ASTPragmaNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);

        convertedNode->selector = std::any_cast<ResultType> (visitNode(*node.selector));
        convertedNode->arguments.reserve(node.arguments.size());
        for(auto &arg : node.arguments)
            convertedNode->arguments.push_back(std::any_cast<ResultType> (visitNode(*arg)));

        return ResultType(convertedNode);
    }
    
    virtual std::any visitBlockNode(ASTBlockNode &node) override
    {
        auto convertedSequence = Environment::staticObjectCast<Environment::ASTSequenceNode> (std::any_cast<ResultType> (visitNode(*node.expressionList)));
        auto blockPosition = convertSourcePosition(node.sourcePosition);

        convertedSequence->pragmas.reserve(node.pragmas.size());
        for(const auto &pragma : node.pragmas)
            convertedSequence->pragmas.push_back(std::any_cast<ResultType> (visitNode(*pragma)));

        if(node.blockClosureSignature)
        {
            auto signature = std::static_pointer_cast<ASTBlockClosureSignatureNode> (node.blockClosureSignature);
            auto blockClosureNode = Environment::basicMakeObject<Environment::ASTClosureNode> ();
            blockClosureNode->sourcePosition = blockPosition;
            blockClosureNode->kind = Environment::ASTClosureNodeKind::Block;
            blockClosureNode->arguments.reserve(signature->arguments.size());
            for(const auto &arg : signature->arguments)
                blockClosureNode->arguments.push_back(std::any_cast<ResultType> (visitNode(*arg)));

            if(signature->returnType)
                blockClosureNode->resultType = std::any_cast<ResultType> (visitNode(*signature->returnType));
            blockClosureNode->body = convertedSequence;
            
            return ResultType(blockClosureNode);
        }
        else
        {
            auto lexicalScope = Environment::basicMakeObject<Environment::ASTLexicalScopeNode> ();
            lexicalScope->sourcePosition = blockPosition;
            lexicalScope->body = convertedSequence;

            auto cleanUpScope = Environment::basicMakeObject<Environment::ASTCleanUpScopeNode> ();
            cleanUpScope->sourcePosition = blockPosition;
            cleanUpScope->body = lexicalScope;
            return ResultType(cleanUpScope);
        }
    }
    
    virtual std::any visitBlockClosureArgumentNode(ASTBlockClosureArgumentNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTArgumentDefinitionNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        if(node.identifier)
            convertedNode->identifier = std::any_cast<ResultType> (visitNode(*node.identifier));
        if(node.type)
            convertedNode->type = std::any_cast<ResultType> (visitNode(*node.type));
        return ResultType(convertedNode);
    }
    
    virtual std::any visitBlockClosureSignatureNode(ASTBlockClosureSignatureNode &) override
    {
        // This should be unreachable.
        return std::any();
    }

    virtual std::any visitIntegerLiteralNode(ASTIntegerLiteralNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitFloatLiteralNode(ASTFloatLiteralNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitCharacterLiteralNode(ASTCharacterLiteralNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitStringLiteralNode(ASTStringLiteralNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitSymbolLiteralNode(ASTSymbolLiteralNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(internSymbol(node.value));
        return ResultType(convertedNode);
    }

    virtual std::any visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTIdentifierReferenceNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->identifier = internSymbol(node.identifier);
        return ResultType(convertedNode);
    }

    virtual std::any visitMessageSendNode(ASTMessageSendNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMessageSendNode> ();
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
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMessageChainNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);

        if(node.receiver)
            convertedNode->receiver = std::any_cast<ResultType> (visitNode(*node.receiver));

        convertedNode->messages.reserve(node.messages.size());
        for(auto &message : node.messages)
            convertedNode->messages.push_back(std::any_cast<Environment::ASTMessageChainMessageNodePtr> (visitNode(*message)));

        return ResultType(convertedNode);
    }

    virtual std::any visitMessageChainMessageNode(ASTMessageChainMessageNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMessageChainMessageNode> ();
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

        auto convertedNode = Environment::basicMakeObject<Environment::ASTCallNode> ();
        convertedNode->sourcePosition = sourcePosition;
        convertedNode->function = std::any_cast<ResultType> (visitNode(*node.callable));

        convertedNode->arguments.reserve(node.arguments.size());
        for(auto &arg : node.arguments)
            convertedNode->arguments.push_back(std::any_cast<ResultType> (visitNode(*arg)));

        return ResultType(convertedNode);
    }

    virtual std::any visitSubscriptNode(ASTSubscriptNode &node) override
    {
        auto sourcePosition = convertSourcePosition(node.sourcePosition);

        auto convertedNode = Environment::basicMakeObject<Environment::ASTMessageSendNode> ();
        convertedNode->sourcePosition = sourcePosition;

        auto selector = Environment::basicMakeObject<Environment::ASTLiteralValueNode>  ();
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
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMakeTupleNode>  ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->elements.reserve(node.elements.size());
        for(auto &element : node.elements)
            convertedNode->elements.push_back(std::any_cast<ResultType> (visitNode(*element)));

        return ResultType(convertedNode);
    }

    virtual std::any visitMakeDictionaryNode(ASTMakeDictionaryNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMakeDictionaryNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->elements.reserve(node.elements.size());
        for(const auto &element : node.elements)
            convertedNode->elements.push_back(std::any_cast<ResultType> (visitNode(*element)));

        return ResultType(convertedNode);
    }

    virtual std::any visitDictionaryElementNode(ASTDictionaryElementNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMakeAssociationNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        if(node.key)
            convertedNode->key = std::any_cast<ResultType> (visitNode(*node.key));
        if(node.value)
            convertedNode->value = std::any_cast<ResultType> (visitNode(*node.value));

        return ResultType(convertedNode);
    }

    virtual std::any visitLiteralArrayNode(ASTLiteralArrayNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMakeLiteralArrayNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->elements.reserve(node.elements.size());
        for(const auto &element : node.elements)
            convertedNode->elements.push_back(std::any_cast<ResultType> (visitNode(*element)));

        return ResultType(convertedNode);
    }

    virtual std::any visitQuoteNode(ASTQuoteNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTQuoteNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expression = std::any_cast<ResultType> (visitNode(*node.quoted));
        return ResultType(convertedNode);
    }

    virtual std::any visitQuasiQuoteNode(ASTQuasiQuoteNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTQuasiQuoteNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expression = std::any_cast<ResultType> (visitNode(*node.quoted));
        return ResultType(convertedNode);
    }

    virtual std::any visitQuasiUnquoteNode(ASTQuasiUnquoteNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTQuasiUnquoteNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expression = std::any_cast<ResultType> (visitNode(*node.expression));
        return ResultType(convertedNode);
    }

    virtual std::any visitSpliceNode(ASTSpliceNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTSpliceNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expression = std::any_cast<ResultType> (visitNode(*node.expression));
        return ResultType(convertedNode);
    }
};

} // End of namespace Sysmel
} // End of namespace Frontend

namespace Environment
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
    return Environment::staticObjectCast<SysmelLanguageSupport> (RuntimeContext::getActive()->getSysmelLanguageSupport());
}

void SysmelLanguageSupport::initialize()
{
    SuperType::initialize();

    {
        keywordScope = Environment::basicMakeObject<LexicalScope> ();
        keywordScope->setSymbolBinding(internSymbol("true"), getTrueConstant());
        keywordScope->setSymbolBinding(internSymbol("false"), getFalseConstant());
        keywordScope->setSymbolBinding(internSymbol("nil"), getNilConstant());
        keywordScope->setSymbolBinding(internSymbol("void"), getVoidConstant());
    }

    {
        topLevelScope = Environment::basicMakeObject<LexicalScope> ();
        topLevelScope->parent = keywordScope;

        // Use the language namespace.
        topLevelScope->useNamespace(RuntimeContext::getActive()->getBootstrapModule()->getEnvironmentSysmelLanguageNamespace());

        // Meta builders
        topLevelScope->setSymbolBinding(internSymbol("let"), metaBuilderFactoryFor<LetMetaBuilder> ("let"));
        topLevelScope->setSymbolBinding(internSymbol("function"), metaBuilderFactoryFor<FunctionMetaBuilder> ("function"));
        topLevelScope->setSymbolBinding(internSymbol("namespace"), metaBuilderFactoryFor<NamespaceMetaBuilder> ("namespace"));

        topLevelScope->setSymbolBinding(internSymbol("struct"), metaBuilderFactoryFor<StructMetaBuilder> ("struct"));
        topLevelScope->setSymbolBinding(internSymbol("union"), metaBuilderFactoryFor<UnionMetaBuilder> ("union"));
        topLevelScope->setSymbolBinding(internSymbol("class"), metaBuilderFactoryFor<ClassMetaBuilder> ("class"));
        topLevelScope->setSymbolBinding(internSymbol("enum"), metaBuilderFactoryFor<EnumMetaBuilder> ("enum"));

        topLevelScope->setSymbolBinding(internSymbol("internal"), metaBuilderFactoryFor<InternalMetaBuilder> ("internal"));
        topLevelScope->setSymbolBinding(internSymbol("private"), metaBuilderFactoryFor<PrivateMetaBuilder> ("private"));
        topLevelScope->setSymbolBinding(internSymbol("protected"), metaBuilderFactoryFor<ProtectedMetaBuilder> ("protected"));
        topLevelScope->setSymbolBinding(internSymbol("public"), metaBuilderFactoryFor<PublicMetaBuilder> ("public"));

        topLevelScope->setSymbolBinding(internSymbol("field"), metaBuilderFactoryFor<FieldMetaBuilder> ("field"));
        topLevelScope->setSymbolBinding(internSymbol("global"), metaBuilderFactoryFor<GlobalMetaBuilder> ("global"));

        topLevelScope->setSymbolBinding(internSymbol("compileTime"), metaBuilderFactoryFor<CompileTimeMetaBuilder> ("compileTime"));

        // Control flow macros.
        topLevelScope->setSymbolBinding(internSymbol("if:then:"), makeBootstrapMethod<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr, ASTNodePtr)> ("if:then:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &condition, const ASTNodePtr &trueExpression) {
            return macroContext->astBuilder->ifThen(condition, trueExpression);
        }, MethodFlags::Macro));
        topLevelScope->setSymbolBinding(internSymbol("if:then:else:"), makeBootstrapMethod<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr, ASTNodePtr, ASTNodePtr)> ("if:then:else:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &condition, const ASTNodePtr &trueExpression, const ASTNodePtr &falseExpression) {
            return macroContext->astBuilder->ifThenElse(condition, trueExpression, falseExpression);
        }, MethodFlags::Macro));

        topLevelScope->setSymbolBinding(internSymbol("while:do:"), makeBootstrapMethod<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr, ASTNodePtr)> ("while:do:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &condition, const ASTNodePtr &bodyExpression) {
            return macroContext->astBuilder->whileDo(condition, bodyExpression);
        }, MethodFlags::Macro));
        topLevelScope->setSymbolBinding(internSymbol("while:do:continueWith:"), makeBootstrapMethod<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr, ASTNodePtr, ASTNodePtr)> ("while:do:continueWith", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &condition, const ASTNodePtr &bodyExpression, const ASTNodePtr &continueExpression) {
            return macroContext->astBuilder->whileDoContinueWith(condition, bodyExpression, continueExpression);
        }, MethodFlags::Macro));

        topLevelScope->setSymbolBinding(internSymbol("do:while:"), makeBootstrapMethod<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr, ASTNodePtr)> ("do:while:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &bodyExpression, const ASTNodePtr &condition) {
            return macroContext->astBuilder->doWhile(bodyExpression, condition);
        }, MethodFlags::Macro));
        topLevelScope->setSymbolBinding(internSymbol("do:while:continueWith:"), makeBootstrapMethod<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr, ASTNodePtr, ASTNodePtr)> ("do:while:continueWith:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &bodyExpression, const ASTNodePtr &condition, const ASTNodePtr &continueExpression) {
            return macroContext->astBuilder->doWhileContinueWith(bodyExpression, condition, continueExpression);
        }, MethodFlags::Macro));

        topLevelScope->setSymbolBinding(internSymbol("return:"), makeBootstrapMethod<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr)> ("return:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &valueExpression) {
            return macroContext->astBuilder->returnValue(valueExpression);
        }, MethodFlags::Macro));

        topLevelScope->setSymbolBinding(internSymbol("break"), makeBootstrapMethod<ASTNodePtr (MacroInvocationContextPtr)> ("break", [](const MacroInvocationContextPtr &macroContext) {
            return macroContext->astBuilder->breakThisLoop();
        }, MethodFlags::Macro));

        topLevelScope->setSymbolBinding(internSymbol("continue"), makeBootstrapMethod<ASTNodePtr (MacroInvocationContextPtr)> ("continue", [](const MacroInvocationContextPtr &macroContext) {
            return macroContext->astBuilder->continueThisLoop();
        }, MethodFlags::Macro));
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
    auto sysmelAST = Sysmel::Frontend::SysmelSyntax::parseString(sourceString, sourceStringName);
    return std::any_cast<ASTNodePtr> (Sysmel::Frontend::SysmelSyntax::SysmelASTConverter().visitNode(*sysmelAST));
}

} // End of namespace Environment
} // End of namespace Sysmel
