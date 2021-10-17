#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTCompileTimeEvaluator.hpp"

#include "sysmel/BootstrapEnvironment/ASTArgumentDefinitionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCleanUpScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTClosureNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCompileTimeEvaluationErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTIdentifierReferenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCallNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLexicalScopeNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeAssociationNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeDictionaryNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeLiteralArrayNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMakeTupleNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainMessageNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageChainNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTPragmaNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuasiQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuasiUnquoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTQuoteNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTParseErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticErrorNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSequenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSpliceNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTLocalVariableNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTVariableAccessNode.hpp"

#include "sysmel/BootstrapEnvironment/ASTNamespaceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTFunctionNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMethodNode.hpp"

#include "sysmel/BootstrapEnvironment/MacroInvocationContext.hpp"
#include "sysmel/BootstrapEnvironment/ASTBuilder.hpp"

#include "sysmel/BootstrapEnvironment/ASTAnalysisEnvironment.hpp"
#include "sysmel/BootstrapEnvironment/CleanUpScope.hpp"
#include "sysmel/BootstrapEnvironment/LexicalScope.hpp"

#include "sysmel/BootstrapEnvironment/ResultTypeInferenceSlot.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/FunctionalType.hpp"
#include "sysmel/BootstrapEnvironment/Namespace.hpp"
#include "sysmel/BootstrapEnvironment/ControlFlowUtilities.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"

#include "sysmel/BootstrapEnvironment/CompilationError.hpp"
#include "sysmel/BootstrapEnvironment/CompilationErrors.hpp"

#include "sysmel/BootstrapEnvironment/LocalVariable.hpp"
#include "sysmel/BootstrapEnvironment/ArgumentVariable.hpp"
#include "sysmel/BootstrapEnvironment/CompiledMethod.hpp"

#include <iostream>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ASTSemanticAnalyzer> ASTSemanticAnalyzerTypeRegistration;

static TypePtr unwrapTypeFromLiteralValue(const ASTNodePtr &node)
{
    assert(node->isASTLiteralTypeNode());
    return std::static_pointer_cast<Type> (
        std::static_pointer_cast<ASTLiteralValueNode> (node)->value
    );
}

ASTNodePtr ASTSemanticAnalyzer::recordSemanticErrorInNode(const ASTNodePtr &errorNode, const std::string &message)
{
    auto semanticErrorNode = std::make_shared<ASTSemanticErrorNode> ();
    semanticErrorNode->sourcePosition = errorNode->sourcePosition;
    semanticErrorNode->errorMessage = message;
    semanticErrorNode->analyzedType = Type::getCompilationErrorValueType();
    recordedErrors.push_back(semanticErrorNode);
    return semanticErrorNode;
}

ASTNodePtr ASTSemanticAnalyzer::recordCompileTimeEvaluationErrorInNode(const ASTNodePtr &errorNode, const ErrorPtr &evaluationError)
{
    auto compileTimeErrorNode = std::make_shared<ASTCompileTimeEvaluationErrorNode> ();
    compileTimeErrorNode->sourcePosition = errorNode->sourcePosition;
    compileTimeErrorNode->errorMessage = evaluationError->getDescription();
    compileTimeErrorNode->caughtError = evaluationError;
    compileTimeErrorNode->analyzedType = Type::getCompilationErrorValueType();
    recordedErrors.push_back(compileTimeErrorNode);
    return compileTimeErrorNode;
}

MacroInvocationContextPtr ASTSemanticAnalyzer::makeMacroInvocationContextFor(const ASTIdentifierReferenceNodePtr &node)
{
    auto result = std::make_shared<MacroInvocationContext> ();
    result->receiverNode = node;
    result->selfType = Type::getVoidType();
    result->astBuilder = std::make_shared<ASTBuilder> ();
    return result;
}

MacroInvocationContextPtr ASTSemanticAnalyzer::makeMacroInvocationContextFor(const ASTMessageSendNodePtr &node)
{
    auto result = std::make_shared<MacroInvocationContext> ();
    result->receiverNode = node->receiver;
    result->selfType = Type::getVoidType();
    if(node->receiver && node->receiver->analyzedType)
        result->selfType = node->receiver->analyzedType;
    result->astBuilder = std::make_shared<ASTBuilder> ();
    return result;
}

ASTNodePtr ASTSemanticAnalyzer::withEnvironmentDoAnalysis(const ASTAnalysisEnvironmentPtr &newEnvironment, const ASTNodeSemanticAnalysisBlock &aBlock)
{
    auto oldEnvironment = environment;
    environment = newEnvironment;

    return doWithEnsure(aBlock, [&](){
        environment = oldEnvironment;
    });
}


ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithTypeInference(const ASTNodePtr &node, const ResultTypeInferenceSlotPtr &typeInferenceSlot, bool concretizeEphemeralObjects)
{
    auto oldExpectedType = currentExpectedType;
    currentExpectedType = typeInferenceSlot;
    
    auto analyzedNode = doWithEnsure([&](){
       return analyzeNodeIfNeededWithCurrentExpectedType(node, concretizeEphemeralObjects);
    },
    [&](){
        currentExpectedType = oldExpectedType;
    });

    // TODO: Add the implicit cast that might be needed here.
    return analyzedNode;
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithExpectedType(const ASTNodePtr &node, const TypePtr &expectedType, bool concretizeEphemeralObjects)
{
    return analyzeNodeIfNeededWithTypeInference(node, ResultTypeInferenceSlot::makeForType(expectedType), concretizeEphemeralObjects);
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithExpectedTypeSet(const ASTNodePtr &node, const TypePtrList &expectedTypeSet)
{
    return analyzeNodeIfNeededWithTypeInference(node, ResultTypeInferenceSlot::makeForTypeSet(expectedTypeSet), true);
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithAutoTypeInferenceMode(const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable)
{
    return analyzeNodeIfNeededWithTypeInference(node, ResultTypeInferenceSlot::makeForAutoWithMode(mode, isMutable), true);
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithAutoType(const ASTNodePtr &node)
{
    return analyzeNodeIfNeededWithTypeInference(node, ResultTypeInferenceSlot::makeForAuto());
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithCurrentExpectedType(const ASTNodePtr &node, bool concretizeEphemeralObjects)
{
    if(node->analyzedType)
        return node;
    
    auto analysisResult = visitNode(node);
    assert(analysisResult->isASTNode());

    auto analysisResultNode = std::static_pointer_cast<ASTNode> (analysisResult);
    assert(analysisResultNode->analyzedType);

    if(concretizeEphemeralObjects && analysisResultNode->isASTLiteralValueNode() && analysisResultNode->analyzedType->isEphemeralCompileTimeObject())
        return analysisResultNode->analyzedType->concretizeEphemeralCompileTimeObject(std::static_pointer_cast<ASTLiteralValueNode> (analysisResultNode), shared_from_this());

    return analysisResultNode;
}

AnyValuePtr ASTSemanticAnalyzer::adaptNodeAsMacroArgumentOfType(const ASTNodePtr &node, const TypePtr &expectedType)
{
    if(node->isKindOf(expectedType))
        return node;
    
    assert("TODO: Support non-node macro parameters" && false);
}

PatternMatchingRank ASTSemanticAnalyzer::rankForMatchingTypeWithValueOfType(const TypePtr &expectedType, const TypePtr &valueType)
{
    // TODO: Introduce support for implicit casting here.
    return expectedType->rankToMatchType(valueType);
}

PatternMatchingRank ASTSemanticAnalyzer::rankForMatchingTypeWithNode(const TypePtr &expectedType, const ASTNodePtr &node)
{
    assert(node->analyzedType);
    return rankForMatchingTypeWithValueOfType(expectedType, node->analyzedType);
}

ASTNodePtr ASTSemanticAnalyzer::analyzeDynamicCompileTimeMessageSendNode(const ASTMessageSendNodePtr &node)
{
    auto anyValueType = AnyValue::__staticType__();
    node->analyzedType = anyValueType;
    node->analyzedBoundMessage.reset();
    node->analyzedBoundMessageIsDirect = false;

    if(node->receiver)
        node->receiver = analyzeNodeIfNeededWithAutoType(node->receiver);
    for(auto &arg : node->arguments)
        arg = analyzeNodeIfNeededWithAutoType(arg);
    return node;
}

ASTNodePtr ASTSemanticAnalyzer::analyzeMessageSendNodeViaDNUMacro(const ASTMessageSendNodePtr &node, const AnyValuePtr &dnuMacro)
{
    assert("TODO: Invoke DNU macro" && false);
}

AnyValuePtr ASTSemanticAnalyzer::evaluateInCompileTime(const ASTNodePtr &node)
{
    auto evaluator = std::make_shared<ASTCompileTimeEvaluator> ();
    return evaluator->visitNode(analyzeNodeIfNeededWithAutoType(node));
}

ASTNodePtr ASTSemanticAnalyzer::guardCompileTimeEvaluationForNode(const ASTNodePtr &node, const ASTNodeSemanticAnalysisBlock &aBlock)
{
    try
    {
        return aBlock();
    }
    catch(ExceptionWrapper &exceptionWrapper)
    {
        auto exception = exceptionWrapper.getException();
        if(exception->isKindOf<Error> ())
            return recordCompileTimeEvaluationErrorInNode(node, std::static_pointer_cast<Error> (exception));

        // Propagate the exception.
        throw exceptionWrapper;
    }
}

ASTNodePtr ASTSemanticAnalyzer::evaluateTypeExpression(const ASTNodePtr &node)
{
    if(node->isASTLiteralTypeNode())
        return node;

    return guardCompileTimeEvaluationForNode(node, [&](){
        auto evaluatedValue = evaluateInCompileTime(node);
        if(!evaluatedValue->isType())
            return recordSemanticErrorInNode(node, "Expected an expression that evaluates to a type.");

        return evaluatedValue->asASTNodeRequiredInPosition(node->sourcePosition);
    });
}

ASTNodePtr ASTSemanticAnalyzer::evaluateLiteralExpressionInCompileTime(const ASTNodePtr &node)
{
    return guardCompileTimeEvaluationForNode(node, [&](){
        return analyzeNodeIfNeededWithCurrentExpectedType(evaluateInCompileTime(node)->asASTNodeRequiredInPosition(node->sourcePosition));
    });
}

ASTNodePtr ASTSemanticAnalyzer::optimizeAnalyzedMessageSend(const ASTMessageSendNodePtr &node)
{
    if(node->isPureCompileTimeLiteralMessage())
        return evaluateLiteralExpressionInCompileTime(node);

    return node;
}

ASTNodePtr ASTSemanticAnalyzer::analyzeCallNodeByConvertingToMessageSend(const ASTCallNodePtr &node)
{
    auto messageSendNode = std::make_shared<ASTMessageSendNode> ();
    messageSendNode->sourcePosition = node->sourcePosition;

    auto selector = std::make_shared<ASTLiteralValueNode> ();
    selector->sourcePosition = node->sourcePosition;
    selector->setValueAndType(internSymbol("()"));

    auto argumentsNode = std::make_shared<ASTMakeTupleNode> ();
    argumentsNode->sourcePosition = node->sourcePosition;
    argumentsNode->elements = node->arguments;

    messageSendNode->selector = selector;
    messageSendNode->receiver = node->function;
    messageSendNode->arguments.push_back(argumentsNode);

    return analyzeNodeIfNeededWithCurrentExpectedType(messageSendNode);
}

AnyValuePtr ASTSemanticAnalyzer::evaluateNameSymbolValue(const ASTNodePtr &node)
{
    assert(node->isASTLiteralValueNode());
    auto result = std::static_pointer_cast<ASTLiteralValueNode> (node)->value;
    return validAnyValue(result)->isAnonymousNameSymbol() ? nullptr : result;
}

bool ASTSemanticAnalyzer::isNameReserved(const AnyValuePtr &name)
{
    return environment->lexicalScope->isNameReserved(name);
}

AnyValuePtr ASTSemanticAnalyzer::visitArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &node)
{
    return recordSemanticErrorInNode(node, "Unexpected location for argument node.");
}

ASTNodePtr ASTSemanticAnalyzer::analyzeArgumentDefinitionNodeWithExpectedType(const ASTArgumentDefinitionNodePtr &node, const TypePtr &expectedType)
{
    auto analyzedNode = std::make_shared<ASTArgumentDefinitionNode> (*node);
    auto name = evaluateNameSymbolValue(analyzedNode->identifier);
    analyzedNode->analyzedIdentifier = name;

    // Analyze the argument type.
    if(analyzedNode->type)
    {
        analyzedNode->type = evaluateTypeExpression(analyzedNode->type);
    }
    else
    {
        if(expectedType)
        {
            analyzedNode->type = expectedType->asASTNodeRequiredInPosition(analyzedNode->sourcePosition);
        }
        else
        {
            if(!environment->defaultArgumentType)
                return recordSemanticErrorInNode(analyzedNode, formatString("Argument {0} requires a explicit type specification.", {{validAnyValue(name)->printString()}}));
            analyzedNode->type = environment->defaultArgumentType->asASTNodeRequiredInPosition(analyzedNode->sourcePosition);
        }
    }


    // Make sure the type is not an error.
    if(analyzedNode->type->isASTErrorNode())
        return analyzedNode->type;

    // Extract the argument type.
    if(!analyzedNode->type->isASTLiteralTypeNode())
        return recordSemanticErrorInNode(analyzedNode, formatString("Failed to define a type for argument {0}.", {{validAnyValue(name)->printString()}}));

    analyzedNode->analyzedType = unwrapTypeFromLiteralValue(analyzedNode->type);

    // Make sure that we have the expected type.
    if(expectedType && (analyzedNode->analyzedType != expectedType))
        return recordSemanticErrorInNode(analyzedNode, formatString("Argument {0} type {1} is not the expected type {2}.", {{
            validAnyValue(name)->printString(),
            analyzedNode->analyzedType->printString(),
            expectedType->printString(),
        }}));

    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node)
{
    auto analyzedNode = std::make_shared<ASTCleanUpScopeNode> (*node);
    analyzedNode->analyzedScope = CleanUpScope::makeWithParent(environment->cleanUpScope);

    auto newEnvironment = environment->copyWithCleanUpcope(analyzedNode->analyzedScope);
    analyzedNode->body = withEnvironmentDoAnalysis(newEnvironment, [&]() {
        return analyzeNodeIfNeededWithCurrentExpectedType(analyzedNode->body);
    });
    
    analyzedNode->analyzedType = analyzedNode->body->analyzedType;
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitClosureNode(const ASTClosureNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node)
{
    auto boundSymbol = environment->lexicalScope->lookupSymbolRecursively(node->identifier);
    if(!boundSymbol)
        return recordSemanticErrorInNode(node, formatString("Failed to find binding for identifier {0}.", {{node->identifier->printString()}}));

    auto analyzedNode = std::make_shared<ASTIdentifierReferenceNode> (*node);
    return boundSymbol->analyzeIdentifierReferenceNode(analyzedNode, shared_from_this());
}

AnyValuePtr ASTSemanticAnalyzer::visitCallNode(const ASTCallNodePtr &node)
{
    auto analyzedNode = std::make_shared<ASTCallNode> (*node);
    analyzedNode->function = analyzeNodeIfNeededWithAutoType(analyzedNode->function);
    return analyzedNode->function->analyzedType->analyzeCallNode(analyzedNode, shared_from_this());
}

ASTNodePtr ASTSemanticAnalyzer::analyzeCallNodeWithFunctionalType(const ASTCallNodePtr &node, const FunctionalTypePtr &functionType)
{
    if(node->arguments.size() != functionType->getArgumentCount())
    {
        // Analyze the arguments for discovering more error.
        for(auto &arg : node->arguments)
            analyzeNodeIfNeededWithAutoType(arg);

        return recordSemanticErrorInNode(node, "Call argument count mismatch.");
    }

    // Analyze the arguments
    for(size_t i = 0; i < node->arguments.size(); ++i)
    {
        auto &arg = node->arguments[i];
        arg = analyzeNodeIfNeededWithExpectedType(arg, functionType->getArgument(i));
    }

    // Make sure there is no receiver type.
    if(!functionType->getReceiverType()->isVoidType())
        return recordSemanticErrorInNode(node, "Called method requires a receiver.");

    node->analyzedType = functionType->getResultType();
    return node;
}

AnyValuePtr ASTSemanticAnalyzer::visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node)
{
    auto analyzedNode = std::make_shared<ASTLexicalScopeNode> (*node);
    analyzedNode->analyzedScope = std::make_shared<LexicalScope> ();
    analyzedNode->analyzedScope->parent = environment->lexicalScope;

    auto newEnvironment = environment->copyWithLexicalScope(analyzedNode->analyzedScope);
    analyzedNode->body = withEnvironmentDoAnalysis(newEnvironment, [&]() {
        return analyzeNodeIfNeededWithCurrentExpectedType(analyzedNode->body);
    });
    
    analyzedNode->analyzedType = analyzedNode->body->analyzedType;
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitLiteralValueNode(const ASTLiteralValueNodePtr &node)
{
    auto result = std::make_shared<ASTLiteralValueNode> (*node);
    result->analyzedType = node->type;
    return result;
}

AnyValuePtr ASTSemanticAnalyzer::visitMakeAssociationNode(const ASTMakeAssociationNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitMakeTupleNode(const ASTMakeTupleNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitMessageChainNode(const ASTMessageChainNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitMessageChainMessageNode(const ASTMessageChainMessageNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitMessageSendNode(const ASTMessageSendNodePtr &node)
{
    auto analyzedNode = std::make_shared<ASTMessageSendNode> (*node);
    analyzedNode->selector = analyzeNodeIfNeededWithAutoType(analyzedNode->selector);

    if(analyzedNode->receiver)
    {
        analyzedNode->receiver = analyzeNodeIfNeededWithAutoType(analyzedNode->receiver);

        // Delegate to the receiver type.
        return analyzedNode->receiver->analyzedType->analyzeMessageSendNode(analyzedNode, shared_from_this());
    }
    else
    {
        if(!analyzedNode->selector->isASTLiteralValueNode())
            return recordSemanticErrorInNode(node, "Cannot analyze message send without receiver that has a non-literal selector.");

        auto literalSelectorNode = std::static_pointer_cast<ASTLiteralValueNode> (analyzedNode->selector);
        auto literalSelector = literalSelectorNode->value;
        auto boundSymbol = environment->lexicalScope->lookupSymbolRecursively(literalSelectorNode->value);
        if(!boundSymbol)
            return recordSemanticErrorInNode(node, formatString("Failed to find a definition for a message without receiver using the {0} selector.", {{literalSelector->printString()}}));

        return boundSymbol->analyzeMessageSendNode(analyzedNode, shared_from_this());
    }
}

AnyValuePtr ASTSemanticAnalyzer::visitParseErrorNode(const ASTParseErrorNodePtr &node)
{
    auto result = std::make_shared<ASTParseErrorNode> (*node);
    result->analyzedType = Type::getCompilationErrorValueType();
    recordedErrors.push_back(result);
    return result;
}

AnyValuePtr ASTSemanticAnalyzer::visitPragmaNode(const ASTPragmaNodePtr &node)
{
    auto analyzedNode = std::make_shared<ASTPragmaNode> (*node);
    analyzedNode->selector = analyzeNodeIfNeededWithExpectedType(analyzedNode->selector, Type::getLiteralSymbolValue());
    for(auto &argument : node->arguments)
        argument = analyzeNodeIfNeededWithExpectedType(argument, Type::getLiteralValueType());

    analyzedNode->analyzedType = Type::getPragmaType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &node)
{
    assert(false);
}

AnyValuePtr ASTSemanticAnalyzer::visitQuasiUnquoteNode(const ASTQuasiUnquoteNodePtr &node)
{
    return recordSemanticErrorInNode(node, "Invalid location for a quasi-unquote expression");
}

AnyValuePtr ASTSemanticAnalyzer::visitQuoteNode(const ASTQuoteNodePtr &node)
{
    auto result = std::make_shared<ASTQuoteNode> (*node);
    result->analyzedType = ASTNode::__staticType__();
    return result;
}

AnyValuePtr ASTSemanticAnalyzer::visitSequenceNode(const ASTSequenceNodePtr &node)
{
    auto analyzedNode = std::make_shared<ASTSequenceNode> (*node);

    // Analyze the pragmas.
    for(auto &pragma : analyzedNode->pragmas)
        pragma = analyzeNodeIfNeededWithExpectedType(pragma, Type::getPragmaType());

    // Analyze the expressions.
    auto voidType = Type::getVoidType();
    if(analyzedNode->expressions.empty())
    {
        analyzedNode->analyzedType = voidType;
    }
    else
    {
        const auto expressionCount = analyzedNode->expressions.size();
        bool hasReturnType = false;

        for(size_t i = 0; i < expressionCount; ++i)
        {
            auto isLast = i + 1 >= expressionCount;
            auto &expression = analyzedNode->expressions[i];

            if(isLast && !hasReturnType)
                expression = analyzeNodeIfNeededWithTypeInference(expression, currentExpectedType, true);
            else
                expression = analyzeNodeIfNeededWithExpectedType(expression, voidType, true);
        }
        analyzedNode->analyzedType = analyzedNode->expressions.back()->analyzedType;
    }

    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitSpliceNode(const ASTSpliceNodePtr &node)
{
    return recordSemanticErrorInNode(node, "Invalid location for a splice expression");
}

AnyValuePtr ASTSemanticAnalyzer::visitLocalVariableNode(const ASTLocalVariableNodePtr &node)
{
    auto analyzedNode = std::make_shared<ASTLocalVariableNode> (*node);

    auto name = evaluateNameSymbolValue(analyzedNode->name);

    if(name)
    {
        // Check the symbol on the current lexical scope.
        {
            auto previousLocalDefinition = environment->lexicalScope->lookupSymbolLocally(name);
            if(previousLocalDefinition)
                return recordSemanticErrorInNode(analyzedNode, formatString("Local variable definition ({1}) overrides previous definition in the same lexical scope ({2}).",
                    {{name->printString(), previousLocalDefinition->printString()}}));
        }

        // Make sure the name is not reserved.
        if(isNameReserved(name))
            return recordSemanticErrorInNode(analyzedNode, formatString("Local variable {1} overrides reserved name.",
                    {{name->printString()}}));
    }

    if(analyzedNode->type)
    {
        analyzedNode->type = evaluateTypeExpression(analyzedNode->type);
    }
    else
    {
        if(analyzedNode->initialValue)
        {
            analyzedNode->initialValue = analyzeNodeIfNeededWithAutoTypeInferenceMode(analyzedNode->initialValue, analyzedNode->typeInferenceMode, analyzedNode->isMutable);
            analyzedNode->type = analyzedNode->initialValue->analyzedType->asASTNodeRequiredInPosition(analyzedNode->sourcePosition);
        }
        else
        {
            assert("TODO: Support deferred and/or default type " && false);    
        }
    }

    auto valueType = Type::getUndefinedType();
    if(analyzedNode->type->isASTLiteralValueNode())
    {
        auto literalTypeNode = std::static_pointer_cast<ASTLiteralValueNode> (analyzedNode->type);
        assert(literalTypeNode->value->isType());

        valueType = std::static_pointer_cast<Type> (literalTypeNode->value);
    }

    // Make sure the initial value is analyzed.
    if(analyzedNode->initialValue)
        analyzedNode->initialValue = analyzeNodeIfNeededWithExpectedType(analyzedNode->initialValue, valueType, true);

    // Create the local variable.
    auto localVariable = std::make_shared<LocalVariable> ();
    localVariable->setDefinitionParameters(name, valueType, analyzedNode->typeInferenceMode, analyzedNode->isMutable);

    // Record the program entity.
    environment->localDefinitionsOwner->recordChildProgramEntityDefinition(localVariable);

    // Bind the local variable in the current lexical scope.
    environment->lexicalScope->setSymbolBinding(name, localVariable);

    // Set it in the analyzed node.
    analyzedNode->analyzedType = localVariable->getReferenceType();
    analyzedNode->analyzedProgramEntity = localVariable;
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitVariableAccessNode(const ASTVariableAccessNodePtr &node)
{
    auto analyzedNode = std::make_shared<ASTVariableAccessNode> (*node);
    auto variable = analyzedNode->variable;
    analyzedNode->analyzedType = analyzedNode->isAccessedByReference ? variable->getReferenceType() : variable->getValueType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitFunctionNode(const ASTFunctionNodePtr &node)
{
    auto analyzedNode = std::make_shared<ASTFunctionNode> (*node);

    // Concretize the default visibility.
    if(analyzedNode->visibility == ProgramEntityVisibility::Default)
        analyzedNode->visibility = ProgramEntityVisibility::LexicalScope;

    // TODO: Use the current expected type to assist the function type inference.

    // Analyze the arguments node.
    bool hasError = false;
    for(size_t i = 0; i < analyzedNode->arguments.size(); ++i)
    {
        auto &arg = analyzedNode->arguments[i];
        assert(arg->isASTArgumentDefinitionNode());
        arg = analyzeArgumentDefinitionNodeWithExpectedType(std::static_pointer_cast<ASTArgumentDefinitionNode> (arg), currentExpectedType->getExpectedFunctionalArgumentType(i));
        hasError = hasError || arg->isASTErrorNode();
    }

    // Evaluate the result type.
    auto expectedResultType = currentExpectedType->getExpectedFunctionalResultType();
    if(!analyzedNode->resultType)
    {
        if(expectedResultType)
        {
            analyzedNode->resultType = expectedResultType->asASTNodeRequiredInPosition(analyzedNode->sourcePosition);
        }
        else if(environment->defaultResultType)
        {
            analyzedNode->resultType = environment->defaultResultType->asASTNodeRequiredInPosition(analyzedNode->sourcePosition);
        }
        else
        {
            analyzedNode->resultType = recordSemanticErrorInNode(analyzedNode, "Function definition requires an explicit result type specification.");
            hasError = true;
        }
    }

    if(analyzedNode->resultType)
    {
        if(!analyzedNode->resultType->isASTErrorNode())
            analyzedNode->resultType = evaluateTypeExpression(analyzedNode->resultType);

        hasError = hasError || analyzedNode->resultType->isASTErrorNode();
    }

    // Evaluate the name.
    auto name = evaluateNameSymbolValue(analyzedNode->name);

    CompiledMethodPtr compiledMethod;
    bool alreadyRegistered = false;
    bool isLocalDefinition = analyzedNode->isRegisteredInLexicalScope();
    auto ownerEntity = isLocalDefinition ? environment->localDefinitionsOwner : environment->programEntityForPublicDefinitions;

    // Check the name.
    if(name)
    {
        // Forbid reversed names.
        if(isNameReserved(name))
            return recordSemanticErrorInNode(analyzedNode, formatString("Function {1} overrides reserved name.", {{name->printString()}}));

        // Check on the lexical scope, or the public entity owner.
        if(isLocalDefinition)
        {
            auto boundSymbol = environment->lexicalScope->lookupSymbolLocally(name);
            if(boundSymbol)
                return recordSemanticErrorInNode(analyzedNode, formatString("Function {1} overrides locally defined symbol ({2}).",
                        {{name->printString(), boundSymbol->printString()}}));
        }
        else
        {
            auto boundSymbol = ownerEntity->lookupLocalSymbolFromScope(name, environment->lexicalScope);
            if(boundSymbol)
            {
                if(!boundSymbol->isCompiledMethod())
                    return recordSemanticErrorInNode(analyzedNode, formatString("Function {1} overrides previous non-namespace definition in its parent program entity ({2}).",
                        {{name->printString(), boundSymbol->printString()}}));

                compiledMethod = std::static_pointer_cast<CompiledMethod> (boundSymbol);
                alreadyRegistered = true;
            }
        }
    }

    // The local definition must have a body
    if(isLocalDefinition && !analyzedNode->body)
        return recordSemanticErrorInNode(analyzedNode, formatString("Locally defined function {1} must have a body.", {{name->printString()}}));

    // Abort the remaining of the function error.
    if(hasError)
    {
        analyzedNode->analyzedType = Type::getCompilationErrorValueType();
        return analyzedNode;
    }

    // Create the function type.
    TypePtrList argumentTypes;
    argumentTypes.reserve(analyzedNode->arguments.size());
    for(const auto &arg : analyzedNode->arguments)
        argumentTypes.push_back(arg->analyzedType);
    auto resultType = unwrapTypeFromLiteralValue(analyzedNode->resultType);

    // Create the compiled method if missing.
    if(!compiledMethod)
    {
        compiledMethod = std::make_shared<CompiledMethod> ();
        compiledMethod->setDeclaration(analyzedNode);

        // If this is a local definition, then define it as a closure.
        if(isLocalDefinition)
            compiledMethod->setClosureSignature(resultType, argumentTypes);
        else
            compiledMethod->setFunctionSignature(resultType, argumentTypes);

        // Set the arguments declaration node.
        for(size_t i = 0; i < analyzedNode->arguments.size(); ++i)
            compiledMethod->setArgumentDeclarationNode(i, std::static_pointer_cast<ASTArgumentDefinitionNode> (analyzedNode->arguments[i]));
    }
    else
    {
        // TODO: Validate the signature for matching types.
    }

    // Set the definition body.
    if(analyzedNode->body)
    {
        // Set the arguments definition node.
        for(size_t i = 0; i < analyzedNode->arguments.size(); ++i)
            compiledMethod->setArgumentDefinitionNode(i, std::static_pointer_cast<ASTArgumentDefinitionNode> (analyzedNode->arguments[i]));

        // Set the definition body.
        compiledMethod->setDefinition(analyzedNode, analyzedNode->body, environment);
    }

    // Register the compiled method.
    if(name)
    {
        if(isLocalDefinition)
        {
            ownerEntity->recordChildProgramEntityDefinition(compiledMethod);
            environment->lexicalScope->setSymbolBinding(name, compiledMethod);
        }
        else if(!alreadyRegistered)
        {
            ownerEntity->recordChildProgramEntityDefinition(compiledMethod);
            ownerEntity->bindProgramEntityWithVisibility(analyzedNode->visibility, compiledMethod);
        }
    }

    // If this is a local definition, analyze in place.
    if(isLocalDefinition)
    {
        auto analyzedBody = compiledMethod->analyzeDefinitionWith(shared_from_this());
        if(analyzedBody->isASTErrorNode())
            return analyzedBody;
    }

    analyzedNode->analyzedProgramEntity = compiledMethod;
    analyzedNode->analyzedType = compiledMethod->getFunctionalType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitNamespaceNode(const ASTNamespaceNodePtr &node)
{
    auto analyzedNode = std::make_shared<ASTNamespaceNode> (*node);

    auto name = evaluateNameSymbolValue(analyzedNode->name);

    // We must be inside another namespace.
    auto ownerEntity = environment->programEntityForPublicDefinitions;
    if(!ownerEntity || !ownerEntity->isNamespace())
        return recordSemanticErrorInNode(analyzedNode, "Namespaces can only be defined inside other namespaces.");

    NamespacePtr namespaceEntity;
    if(name)
    {
        if(isNameReserved(name))
            return recordSemanticErrorInNode(analyzedNode, formatString("Namespace {1} overrides reserved name.",
                    {{name->printString()}}));

        // Find an existing namespace with the same name.
        {
            auto boundSymbol = ownerEntity->lookupLocalSymbolFromScope(name, environment->lexicalScope);
            if(boundSymbol)
            {
                if(!boundSymbol->isNamespace())
                    return recordSemanticErrorInNode(analyzedNode, formatString("Namespace ({1}) overrides previous non-namespace definition in its parent namespace ({2}).",
                        {{name->printString(), boundSymbol->printString()}}));

                namespaceEntity = std::static_pointer_cast<Namespace> (boundSymbol);
            }
        }

        // Check the symbol on the current lexical scope.
        if(!namespaceEntity)
        {
            auto previousLocalDefinition = environment->lexicalScope->lookupSymbolLocally(name);
            if(previousLocalDefinition)
                return recordSemanticErrorInNode(analyzedNode, formatString("Namespace ({1}) overrides previous definition in the same lexical scope ({2}).",
                    {{name->printString(), previousLocalDefinition->printString()}}));
        }
    }

    // Create the namespace if required.
    if(!namespaceEntity)
    {
        namespaceEntity = Namespace::makeWithName(name);
        ownerEntity->recordChildProgramEntityDefinition(namespaceEntity);
        if(name)
            ownerEntity->bindProgramEntityWithVisibility(ProgramEntityVisibility::Public, namespaceEntity);
    }

    // If the namespace is anonymous, use it in the current lexical scope.
    if(!name)
        environment->lexicalScope->useNamespace(namespaceEntity);

    // Analyze the body.
    if(analyzedNode->body)
        analyzedNode->body = withEnvironmentDoAnalysis(environment->copyForPublicProgramEntityBody(namespaceEntity), [&]() {
            return analyzeNodeIfNeededWithExpectedType(analyzedNode->body, Type::getVoidType());
        });

    // Finish the node analysis.
    analyzedNode->analyzedProgramEntity = namespaceEntity;
    analyzedNode->analyzedType = Type::getNamespaceType();
    return analyzedNode;
}

CompilationErrorPtr ASTSemanticAnalyzer::makeCompilationError()
{
    if(recordedErrors.empty())
        return nullptr;

    if(recordedErrors.size() == 1)
        return recordedErrors.back()->asCompilationError();

    auto errors = std::make_shared<CompilationErrors> ();
    for(const auto &node : recordedErrors)
        errors->errors.push_back(node->asCompilationError());

    return errors;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius