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
#include "Environment/ASTPatternMatchingNode.hpp"

#include <fstream>

namespace Sysmel
{
namespace Frontend
{
namespace SysmelSyntax
{

using Environment::wrapValue;
using Environment::internSymbol;

static Environment::ASTNodePtr castToNode(const AnyValuePtr &value)
{
    sysmelAssert(value && value->isASTNode());
    return Environment::staticObjectCast<Environment::ASTNode> (value);
}

class SysmelASTConverter : public ASTVisitor
{
public:
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

    virtual AnyValuePtr visitParseErrorNode(ASTParseErrorNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTParseErrorNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->errorMessage = node.errorMessage;
        return convertedNode;
    }

    virtual AnyValuePtr visitExpressionListNode(ASTExpressionListNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTSequenceNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expressions.reserve(node.expressions.size());
        for(auto expr : node.expressions)
            convertedNode->expressions.push_back(castToNode(visitNode(*expr)));

        return convertedNode;
    }
    
    virtual AnyValuePtr visitPragmaNode(ASTPragmaNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTPragmaNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);

        convertedNode->selector = castToNode(visitNode(*node.selector));
        convertedNode->arguments.reserve(node.arguments.size());
        for(auto &arg : node.arguments)
            convertedNode->arguments.push_back(castToNode(visitNode(*arg)));

        return convertedNode;
    }
    
    virtual AnyValuePtr visitBlockNode(ASTBlockNode &node) override
    {
        auto convertedSequence = Environment::staticObjectCast<Environment::ASTSequenceNode> (visitNode(*node.expressionList));
        auto blockPosition = convertSourcePosition(node.sourcePosition);

        convertedSequence->pragmas.reserve(node.pragmas.size());
        for(const auto &pragma : node.pragmas)
            convertedSequence->pragmas.push_back(castToNode(visitNode(*pragma)));

        if(node.blockClosureSignature)
        {
            auto signature = std::static_pointer_cast<ASTBlockClosureSignatureNode> (node.blockClosureSignature);
            auto blockClosureNode = Environment::basicMakeObject<Environment::ASTClosureNode> ();
            blockClosureNode->sourcePosition = blockPosition;
            blockClosureNode->kind = Environment::ASTClosureNodeKind::Block;
            blockClosureNode->arguments.reserve(signature->arguments.size());
            for(const auto &arg : signature->arguments)
                blockClosureNode->arguments.push_back(castToNode(visitNode(*arg)));

            if(signature->returnType)
                blockClosureNode->resultType = castToNode(visitNode(*signature->returnType));
            blockClosureNode->body = convertedSequence;
            
            return blockClosureNode;
        }
        else
        {
            auto lexicalScope = Environment::basicMakeObject<Environment::ASTLexicalScopeNode> ();
            lexicalScope->sourcePosition = blockPosition;
            lexicalScope->body = convertedSequence;

            auto cleanUpScope = Environment::basicMakeObject<Environment::ASTCleanUpScopeNode> ();
            cleanUpScope->sourcePosition = blockPosition;
            cleanUpScope->body = lexicalScope;
            return cleanUpScope;
        }
    }
    
    virtual AnyValuePtr visitBlockClosureArgumentNode(ASTBlockClosureArgumentNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTArgumentDefinitionNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        if(node.identifier)
            convertedNode->identifier = castToNode(visitNode(*node.identifier));
        if(node.type)
            convertedNode->type = castToNode(visitNode(*node.type));
        return convertedNode;
    }
    
    virtual AnyValuePtr visitBlockClosureSignatureNode(ASTBlockClosureSignatureNode &) override
    {
        // This should be unreachable.
        return nullptr;
    }

    virtual AnyValuePtr visitIntegerLiteralNode(ASTIntegerLiteralNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return convertedNode;
    }

    virtual AnyValuePtr visitFloatLiteralNode(ASTFloatLiteralNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return convertedNode;
    }

    virtual AnyValuePtr visitCharacterLiteralNode(ASTCharacterLiteralNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return convertedNode;
    }

    virtual AnyValuePtr visitStringLiteralNode(ASTStringLiteralNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(wrapValue(node.value));
        return convertedNode;
    }

    virtual AnyValuePtr visitSymbolLiteralNode(ASTSymbolLiteralNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTLiteralValueNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->setValueAndType(internSymbol(node.value));
        return convertedNode;
    }

    virtual AnyValuePtr visitIdentifierReferenceNode(ASTIdentifierReferenceNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTIdentifierReferenceNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->identifier = internSymbol(node.identifier);
        return convertedNode;
    }

    virtual AnyValuePtr visitMessageSendNode(ASTMessageSendNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMessageSendNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);

        if(node.receiver)
            convertedNode->receiver = castToNode(visitNode(*node.receiver));

        convertedNode->selector = castToNode(visitNode(*node.selector));

        convertedNode->arguments.reserve(node.arguments.size());
        for(auto &arg : node.arguments)
            convertedNode->arguments.push_back(castToNode(visitNode(*arg)));

        return convertedNode;
    }

    virtual AnyValuePtr visitMessageChainNode(ASTMessageChainNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMessageChainNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);

        if(node.receiver)
            convertedNode->receiver = castToNode(visitNode(*node.receiver));

        convertedNode->messages.reserve(node.messages.size());
        for(auto &message : node.messages)
            convertedNode->messages.push_back(castToNode(visitNode(*message)));

        return convertedNode;
    }

    virtual AnyValuePtr visitMessageChainMessageNode(ASTMessageChainMessageNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMessageChainMessageNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);

        convertedNode->selector = castToNode(visitNode(*node.selector));

        convertedNode->arguments.reserve(node.arguments.size());
        for(auto &arg : node.arguments)
            convertedNode->arguments.push_back(castToNode(visitNode(*arg)));

        return convertedNode;
    }

    virtual AnyValuePtr visitCallNode(ASTCallNode &node) override
    {
        auto sourcePosition = convertSourcePosition(node.sourcePosition);

        auto convertedNode = Environment::basicMakeObject<Environment::ASTCallNode> ();
        convertedNode->sourcePosition = sourcePosition;
        convertedNode->function = castToNode(visitNode(*node.callable));

        convertedNode->arguments.reserve(node.arguments.size());
        for(auto &arg : node.arguments)
            convertedNode->arguments.push_back(castToNode(visitNode(*arg)));

        return convertedNode;
    }

    virtual AnyValuePtr visitSubscriptNode(ASTSubscriptNode &node) override
    {
        auto sourcePosition = convertSourcePosition(node.sourcePosition);

        auto convertedNode = Environment::basicMakeObject<Environment::ASTMessageSendNode> ();
        convertedNode->sourcePosition = sourcePosition;

        auto selector = Environment::basicMakeObject<Environment::ASTLiteralValueNode>  ();
        selector->sourcePosition = sourcePosition;
        selector->setValueAndType(internSymbol("[]"));

        convertedNode->receiver = castToNode(visitNode(*node.array));
        convertedNode->selector = selector;

        // Pass the optional index.
        if(node.index)
        {
            convertedNode->arguments.reserve(1);
            convertedNode->arguments.push_back(castToNode(visitNode(*node.index)));
        }

        return convertedNode;
    }

    virtual AnyValuePtr visitMakeTupleNode(ASTMakeTupleNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMakeTupleNode>  ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->elements.reserve(node.elements.size());
        for(auto &element : node.elements)
            convertedNode->elements.push_back(castToNode(visitNode(*element)));

        return convertedNode;
    }

    virtual AnyValuePtr visitMakeDictionaryNode(ASTMakeDictionaryNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMakeDictionaryNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->elements.reserve(node.elements.size());
        for(const auto &element : node.elements)
            convertedNode->elements.push_back(castToNode(visitNode(*element)));

        return convertedNode;
    }

    virtual AnyValuePtr visitDictionaryElementNode(ASTDictionaryElementNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMakeAssociationNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        if(node.key)
            convertedNode->key = castToNode(visitNode(*node.key));
        if(node.value)
            convertedNode->value = castToNode(visitNode(*node.value));

        return convertedNode;
    }

    virtual AnyValuePtr visitLiteralArrayNode(ASTLiteralArrayNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTMakeLiteralArrayNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->elements.reserve(node.elements.size());
        for(const auto &element : node.elements)
            convertedNode->elements.push_back(castToNode(visitNode(*element)));

        return convertedNode;
    }

    virtual AnyValuePtr visitQuoteNode(ASTQuoteNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTQuoteNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expression = castToNode(visitNode(*node.quoted));
        return convertedNode;
    }

    virtual AnyValuePtr visitQuasiQuoteNode(ASTQuasiQuoteNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTQuasiQuoteNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expression = castToNode(visitNode(*node.quoted));
        return convertedNode;
    }

    virtual AnyValuePtr visitQuasiUnquoteNode(ASTQuasiUnquoteNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTQuasiUnquoteNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expression = castToNode(visitNode(*node.expression));
        return convertedNode;
    }

    virtual AnyValuePtr visitSpliceNode(ASTSpliceNode &node) override
    {
        auto convertedNode = Environment::basicMakeObject<Environment::ASTSpliceNode> ();
        convertedNode->sourcePosition = convertSourcePosition(node.sourcePosition);
        convertedNode->expression = castToNode(visitNode(*node.expression));
        return convertedNode;
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
        keywordScope = LexicalScope::makeEmpty(ASTSourcePosition::empty());
        keywordScope->setSymbolBinding(internSymbol("true"), getTrueConstant());
        keywordScope->setSymbolBinding(internSymbol("false"), getFalseConstant());
        keywordScope->setSymbolBinding(internSymbol("nil"), getNilConstant());
        keywordScope->setSymbolBinding(internSymbol("void"), getVoidConstant());
    }

    {
        topLevelScope = LexicalScope::makeEmpty(ASTSourcePosition::empty());
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

        topLevelScope->setSymbolBinding(internSymbol("match:withPatterns:"), makeBootstrapMethod<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr, ASTNodePtr)> ("return:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &valueExpression, const ASTNodePtr &casesExpression) {
            return macroContext->astBuilder->patternMatchingWithCases(valueExpression, casesExpression);
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
    return staticObjectCast<ASTNode> (Sysmel::Frontend::SysmelSyntax::SysmelASTConverter().visitNode(*sysmelAST));
}

} // End of namespace Environment
} // End of namespace Sysmel
