#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTCompileTimeEvaluator.hpp"

#include "Environment/ASTArgumentDefinitionNode.hpp"
#include "Environment/ASTCleanUpScopeNode.hpp"
#include "Environment/ASTClosureNode.hpp"
#include "Environment/ASTCompileTimeEvaluationErrorNode.hpp"
#include "Environment/ASTIdentifierReferenceNode.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTLexicalScopeNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTMakeAggregateNode.hpp"
#include "Environment/ASTMakeAssociationNode.hpp"
#include "Environment/ASTMakeDictionaryNode.hpp"
#include "Environment/ASTMakeLiteralArrayNode.hpp"
#include "Environment/ASTMakeTupleNode.hpp"
#include "Environment/ASTMakeVariantNode.hpp"
#include "Environment/ASTMessageChainMessageNode.hpp"
#include "Environment/ASTMessageChainNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTPragmaNode.hpp"
#include "Environment/ASTQuasiQuoteNode.hpp"
#include "Environment/ASTQuasiUnquoteNode.hpp"
#include "Environment/ASTQuoteNode.hpp"
#include "Environment/ASTParseErrorNode.hpp"
#include "Environment/ASTSemanticErrorNode.hpp"
#include "Environment/ASTSequenceNode.hpp"
#include "Environment/ASTSpliceNode.hpp"

#include "Environment/ASTLocalVariableNode.hpp"
#include "Environment/ASTGlobalVariableNode.hpp"
#include "Environment/ASTFieldVariableNode.hpp"
#include "Environment/ASTCompileTimeConstantNode.hpp"

#include "Environment/ASTFieldVariableAccessNode.hpp"
#include "Environment/ASTSlotAccessNode.hpp"
#include "Environment/ASTVariableAccessNode.hpp"

#include "Environment/ASTNamespaceNode.hpp"
#include "Environment/ASTFunctionNode.hpp"
#include "Environment/ASTMethodNode.hpp"

#include "Environment/ASTEnumNode.hpp"
#include "Environment/ASTStructNode.hpp"
#include "Environment/ASTUnionNode.hpp"
#include "Environment/ASTClassNode.hpp"
#include "Environment/ASTTemplateNode.hpp"
#include "Environment/ASTProgramEntityExtensionNode.hpp"

#include "Environment/ASTExplicitCastNode.hpp"
#include "Environment/ASTImplicitCastNode.hpp"
#include "Environment/ASTReinterpretCastNode.hpp"

#include "Environment/ASTTypeConversionNode.hpp"

#include "Environment/ASTIfNode.hpp"
#include "Environment/ASTWhileNode.hpp"
#include "Environment/ASTDoWhileNode.hpp"
#include "Environment/ASTReturnNode.hpp"
#include "Environment/ASTBreakNode.hpp"
#include "Environment/ASTContinueNode.hpp"

#include "Environment/MacroInvocationContext.hpp"
#include "Environment/ASTBuilder.hpp"

#include "Environment/ASTAnalysisEnvironment.hpp"
#include "Environment/CleanUpScope.hpp"
#include "Environment/LexicalScope.hpp"

#include "Environment/ResultTypeInferenceSlot.hpp"
#include "Environment/Type.hpp"
#include "Environment/FunctionalType.hpp"
#include "Environment/Namespace.hpp"
#include "Environment/ControlFlowUtilities.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/StringUtilities.hpp"

#include "Environment/CompilationError.hpp"
#include "Environment/CompilationErrors.hpp"

#include "Environment/LocalVariable.hpp"
#include "Environment/GlobalVariable.hpp"
#include "Environment/FieldVariable.hpp"
#include "Environment/ArgumentVariable.hpp"
#include "Environment/CompiledMethod.hpp"
#include "Environment/Template.hpp"
#include "Environment/CompileTimeConstant.hpp"

#include "Environment/EnumType.hpp"
#include "Environment/EnumRawValueLookupScope.hpp"
#include "Environment/ClassType.hpp"
#include "Environment/StructureType.hpp"
#include "Environment/UnionType.hpp"
#include "Environment/TupleType.hpp"
#include "Environment/VariantType.hpp"
#include "Environment/AggregateTypeLayout.hpp"

#include "Environment/DeferredCompileTimeCodeFragment.hpp"

#include "Environment/TypeConversionRule.hpp"

#include "Environment/MessageChainReceiverName.hpp"

#include "Environment/LiteralArray.hpp"
#include "Environment/LiteralAssociation.hpp"
#include "Environment/LiteralDictionary.hpp"
#include "Environment/LanguageSupport.hpp"
#include "Environment/LiteralBoolean.hpp"
#include "Environment/LiteralInteger.hpp"
#include "Environment/PrimitiveBooleanType.hpp"
#include "Environment/PrimitiveNumberType.hpp"

#include "Environment/ASTQuasiQuoteAnalyzer.hpp"
#include "Environment/ASTQuasiQuotePatternExpansionNode.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ASTSemanticAnalyzer> ASTSemanticAnalyzerTypeRegistration;

static TypePtr unwrapTypeFromLiteralValue(const ASTNodePtr &node)
{
    sysmelAssert(node->isASTLiteralTypeNode());
    return staticObjectCast<Type> (
        node.staticAs<ASTLiteralValueNode> ()->value
    );
}

ASTNodePtr ASTSemanticAnalyzer::recordSemanticErrorInNode(const ASTNodePtr &errorNode, const std::string &message)
{
    auto semanticErrorNode = basicMakeObject<ASTSemanticErrorNode> ();
    semanticErrorNode->sourcePosition = errorNode->sourcePosition;
    semanticErrorNode->errorMessage = message;
    semanticErrorNode->analyzedType = Type::getCompilationErrorValueType();
    recordedErrors.push_back(semanticErrorNode);
    return semanticErrorNode;
}

ASTNodePtr ASTSemanticAnalyzer::recordCompileTimeEvaluationErrorInNode(const ASTNodePtr &errorNode, const ErrorPtr &evaluationError)
{
    auto compileTimeErrorNode = basicMakeObject<ASTCompileTimeEvaluationErrorNode> ();
    compileTimeErrorNode->sourcePosition = errorNode->sourcePosition;
    compileTimeErrorNode->errorMessage = evaluationError->getDescription();
    compileTimeErrorNode->caughtError = evaluationError;
    compileTimeErrorNode->analyzedType = Type::getCompilationErrorValueType();
    recordedErrors.push_back(compileTimeErrorNode);
    return compileTimeErrorNode;
}

MacroInvocationContextPtr ASTSemanticAnalyzer::makeMacroInvocationContextFor(const ASTIdentifierReferenceNodePtr &node)
{
    auto result = basicMakeObject<MacroInvocationContext> ();
    result->receiverNode = node;
    result->selfType = Type::getVoidType();
    result->astBuilder = basicMakeObject<ASTBuilder> ();
    result->astBuilder->sourcePosition = node->sourcePosition;
    return result;
}

MacroInvocationContextPtr ASTSemanticAnalyzer::makeMacroInvocationContextFor(const ASTMessageSendNodePtr &node)
{
    auto result = basicMakeObject<MacroInvocationContext> ();
    result->receiverNode = node->receiver;
    result->selfType = Type::getVoidType();
    if(node->receiver && node->receiver->analyzedType)
        result->selfType = node->receiver->analyzedType;
    result->astBuilder = basicMakeObject<ASTBuilder> ();
    result->astBuilder->sourcePosition = node->sourcePosition;
    return result;
}

MacroInvocationContextPtr ASTSemanticAnalyzer::makeMacroInvocationContextFor(const ASTNodePtr &node)
{
    if(node->isASTMessageSendNode())
        return makeMacroInvocationContextFor(staticObjectCast<ASTMessageSendNode> (node));
    else if(node->isASTIdentifierReferenceNode())
        return makeMacroInvocationContextFor(staticObjectCast<ASTIdentifierReferenceNode> (node));

    auto result = basicMakeObject<MacroInvocationContext> ();
    result->receiverNode = node;
    result->selfType = Type::getVoidType();
    result->astBuilder = basicMakeObject<ASTBuilder> ();
    result->astBuilder->sourcePosition = node->sourcePosition;
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

    return typeInferenceSlot->concretizeTypeInferenceOfNodeWith(analyzedNode, selfFromThis());
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithExpectedType(const ASTNodePtr &node, const TypePtr &expectedType, bool concretizeEphemeralObjects)
{
    return analyzeNodeIfNeededWithTypeInference(node, ResultTypeInferenceSlot::makeForType(expectedType), concretizeEphemeralObjects);
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithExpectedReceiverType(const ASTNodePtr &node, const TypePtr &expectedType, bool concretizeEphemeralObjects)
{
    return analyzeNodeIfNeededWithTypeInference(node, ResultTypeInferenceSlot::makeForReceiverType(expectedType), concretizeEphemeralObjects);
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithExpectedTypeSet(const ASTNodePtr &node, const TypePtrList &expectedTypeSet)
{
    return analyzeNodeIfNeededWithTypeInference(node, ResultTypeInferenceSlot::makeForTypeSet(expectedTypeSet), true);
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithAutoTypeInferenceMode(const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals)
{
    return analyzeNodeIfNeededWithTypeInference(node, ResultTypeInferenceSlot::makeForAutoWithMode(mode, isMutable, concreteLiterals), true);
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithAutoType(const ASTNodePtr &node)
{
    return analyzeNodeIfNeededWithTypeInference(node, ResultTypeInferenceSlot::makeForAuto());
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithTemporaryAutoType(const ASTNodePtr &node)
{
    return analyzeNodeIfNeededWithTypeInference(node, ResultTypeInferenceSlot::makeForTemporaryAuto());
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithBooleanExpectedType(const ASTNodePtr &node)
{
    auto partiallyAnalyzedNode = analyzeNodeIfNeededWithAutoType(node);
    if(partiallyAnalyzedNode->isASTErrorNode())
        return partiallyAnalyzedNode;
    
    // Are we a part of the literal boolean hierarchy?
    auto type = partiallyAnalyzedNode->analyzedType;
    auto literalBooleanType = LiteralBoolean::__staticType__();
    if(node->isASTLiteralValueNode())
        return addImplicitCastTo(partiallyAnalyzedNode, Boolean8::__staticType__());

    if(type->isSubtypeOf(literalBooleanType) || literalBooleanType->isSubtypeOf(type))
        return partiallyAnalyzedNode;

    // Convert onto boolean8
    return addImplicitCastTo(partiallyAnalyzedNode, Boolean8::__staticType__());
}

ASTNodePtr ASTSemanticAnalyzer::analyzeNodeIfNeededWithCurrentExpectedType(const ASTNodePtr &node, bool concretizeEphemeralObjects)
{
    if(node->analyzedType)
        return node;
    
    auto analysisResult = visitNode(node);
    sysmelAssert(analysisResult->isASTNode());

    auto analysisResultNode = staticObjectCast<ASTNode> (analysisResult);
    sysmelAssert(analysisResultNode->analyzedType);

    if(concretizeEphemeralObjects && analysisResultNode->isASTLiteralValueNode() && analysisResultNode->analyzedType->isEphemeralCompileTimeObject())
        return analysisResultNode->analyzedType->concretizeEphemeralCompileTimeObject(staticObjectCast<ASTLiteralValueNode> (analysisResultNode), selfFromThis());

    return analysisResultNode;
}

AnyValuePtr ASTSemanticAnalyzer::adaptNodeAsMacroArgumentOfType(const ASTNodePtr &node, const TypePtr &expectedType)
{
    if(node->isKindOf(expectedType))
        return node;

    auto analyzedNode = analyzeNodeIfNeededWithExpectedType(node, expectedType);
    if(analyzedNode->isASTErrorNode())
        return analyzedNode;

    // TODO: Guard this compile time evaluation.
    return evaluateInCompileTime(analyzedNode);
}

PatternMatchingRank ASTSemanticAnalyzer::rankForMatchingTypeWithNode(const TypePtr &expectedType, const ASTNodePtr &node)
{
    auto typeConversionRule = node->analyzedType->findImplicitTypeConversionRuleForInto(node, expectedType);
    if(!typeConversionRule)
        return -1;

    return PatternMatchingRank(typeConversionRule->getConversionCost(node, expectedType));
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
    sysmelAssert("TODO: Invoke DNU macro" && false);
}

AnyValuePtr ASTSemanticAnalyzer::evaluateInCompileTime(const ASTNodePtr &node)
{
    auto evaluator = basicMakeObject<ASTCompileTimeEvaluator> ();
    return evaluator->visitNodeCachingExplicitReturns(analyzeNodeIfNeededWithTemporaryAutoType(node));
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
            return recordCompileTimeEvaluationErrorInNode(node, staticObjectCast<Error> (exception));

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
    auto messageSendNode = basicMakeObject<ASTMessageSendNode> ();
    messageSendNode->sourcePosition = node->sourcePosition;

    auto selector = basicMakeObject<ASTLiteralValueNode> ();
    selector->sourcePosition = node->sourcePosition;
    selector->setValueAndType(internSymbol("()"));

    auto argumentsNode = basicMakeObject<ASTMakeTupleNode> ();
    argumentsNode->sourcePosition = node->sourcePosition;
    argumentsNode->elements = node->arguments;

    messageSendNode->selector = selector;
    messageSendNode->receiver = node->function;
    messageSendNode->arguments.push_back(argumentsNode);

    return analyzeNodeIfNeededWithCurrentExpectedType(messageSendNode);
}

AnyValuePtr ASTSemanticAnalyzer::evaluateNameSymbolValue(const ASTNodePtr &node)
{
    if(!node)
        return nullptr;

    sysmelAssert(node->isASTLiteralValueNode());
    auto result = node.staticAs<ASTLiteralValueNode> ()->value;
    return validAnyValue(result)->isAnonymousNameSymbol() ? nullptr : result;
}

bool ASTSemanticAnalyzer::isNameReserved(const AnyValuePtr &name)
{
    return environment->lexicalScope->isNameReserved(name);
}

ASTNodePtr ASTSemanticAnalyzer::addImplicitCastTo(const ASTNodePtr &node, const TypePtr &targetType, bool isReceiverType)
{
    auto analyzedNode = node;
    if(!analyzedNode->analyzedType)
    {
        if(isReceiverType)
            analyzedNode = analyzeNodeIfNeededWithExpectedReceiverType(node, targetType);
        else
            analyzedNode = analyzeNodeIfNeededWithExpectedType(node, targetType);
    }
    if(analyzedNode->isASTErrorNode())
        return analyzedNode;

    // If this is a control flow escape, then just return the node.
    if(analyzedNode->analyzedType->isControlFlowEscapeType())
        return analyzedNode;

    auto sourceType = analyzedNode->analyzedType;
    auto typeConversionRule = sourceType->findImplicitTypeConversionRuleForInto(analyzedNode, targetType, isReceiverType);
    if(!typeConversionRule)
        return recordSemanticErrorInNode(analyzedNode, formatString("Cannot perform implicit cast from '{0}' onto '{1}'.", {sourceType->printString(), targetType->printString()}));
    
    return typeConversionRule->convertNodeAtIntoWith(analyzedNode, node->sourcePosition, targetType, selfFromThis());
}

ASTNodePtr ASTSemanticAnalyzer::addImplicitCastToOneOf(const ASTNodePtr &node, const TypePtrList &expectedTypeSet, bool isReceiverType)
{
    auto analyzedNode = node;
    if(!analyzedNode->analyzedType)
        analyzedNode = analyzeNodeIfNeededWithTemporaryAutoType(node);

    // If this is a control flow escape, then just return the node.
    if(analyzedNode->analyzedType->isControlFlowEscapeType())
        return analyzedNode;

    sysmelAssert(!expectedTypeSet.empty());
    if(expectedTypeSet.size() == 1)
        return addImplicitCastTo(node, expectedTypeSet[0], isReceiverType);

    // Is there a single matching type?
    size_t bestConversionRuleCost = std::numeric_limits<std::size_t>::max();
    std::vector<std::pair<TypeConversionRulePtr, TypePtr>> bestConversionRules;
    auto &sourceType = node->analyzedType;
    for(auto &expectedType : expectedTypeSet)
    {
        auto rule = sourceType->findImplicitTypeConversionRuleForInto(node, expectedType);
        if(!rule)
            continue;
        
        auto cost = rule->getConversionCost(node, expectedType);
        if(cost < bestConversionRuleCost)
        {
            bestConversionRuleCost = cost;
            bestConversionRules.clear();
            bestConversionRules.push_back({rule, expectedType});
        }
        else if(cost == bestConversionRuleCost)
        {
            bestConversionRules.push_back({rule, expectedType});
        }
    }

    // Single conversion rule found.
    if(bestConversionRules.size() == 1)
        return bestConversionRules[0].first->convertNodeAtIntoWith(node, node->sourcePosition, bestConversionRules[0].second, selfFromThis());

    // No conversion, or ambiguous conversion rule found.
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &node)
{
    return recordSemanticErrorInNode(node, "Unexpected location for argument node.");
}

ASTNodePtr ASTSemanticAnalyzer::analyzeArgumentDefinitionNodeWithExpectedType(const ASTArgumentDefinitionNodePtr &node, const TypePtr &expectedType, bool isMacro)
{
    auto analyzedNode = shallowCloneObject(node);
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
            auto defaultArgumentType = isMacro ? ASTNode::__staticType__() : environment->defaultArgumentType;
            if(!defaultArgumentType)
                return recordSemanticErrorInNode(analyzedNode, formatString("Argument {0} requires a explicit type specification.", {{validAnyValue(name)->printString()}}));
            analyzedNode->type = defaultArgumentType->asASTNodeRequiredInPosition(analyzedNode->sourcePosition);
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

ASTNodePtr ASTSemanticAnalyzer::analyzeTemplateArgumentDefinitionNode(const ASTArgumentDefinitionNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    auto name = evaluateNameSymbolValue(analyzedNode->identifier);
    analyzedNode->analyzedIdentifier = name;

    // Analyze the argument type.
    if(analyzedNode->type)
    {
        analyzedNode->type = evaluateTypeExpression(analyzedNode->type);
    }
    else
    {
        if(!environment->defaultTemplateArgumentType)
            return recordSemanticErrorInNode(analyzedNode, formatString("Template argument {0} requires a explicit type specification.", {{validAnyValue(name)->printString()}}));
        analyzedNode->type = environment->defaultArgumentType->asASTNodeRequiredInPosition(analyzedNode->sourcePosition);
    }


    // Make sure the type is not an error.
    if(analyzedNode->type->isASTErrorNode())
        return analyzedNode->type;

    // Extract the argument type.
    if(!analyzedNode->type->isASTLiteralTypeNode())
        return recordSemanticErrorInNode(analyzedNode, formatString("Failed to define a type for argument {0}.", {{validAnyValue(name)->printString()}}));

    analyzedNode->analyzedType = unwrapTypeFromLiteralValue(analyzedNode->type);
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitCleanUpScopeNode(const ASTCleanUpScopeNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
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
    auto analyzedNode = shallowCloneObject(node);

    // TODO: Use the current expected type to assist the function type inference.

    // Analyze the arguments node.
    bool hasError = false;
    for(size_t i = 0; i < analyzedNode->arguments.size(); ++i)
    {
        auto &arg = analyzedNode->arguments[i];
        sysmelAssert(arg->isASTArgumentDefinitionNode());
        arg = analyzeArgumentDefinitionNodeWithExpectedType(staticObjectCast<ASTArgumentDefinitionNode> (arg), currentExpectedType->getExpectedFunctionalArgumentType(i));
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

    CompiledMethodPtr compiledMethod;
    auto ownerEntity = environment->localDefinitionsOwner;

    // Add a default body if it is not present.
    if(!analyzedNode->body)
        analyzedNode->body = getVoidConstant()->asASTNodeRequiredInPosition(analyzedNode->sourcePosition);

    // Abort the remaining analysis of the closure on error.
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
    compiledMethod = basicMakeObject<CompiledMethod> ();
    compiledMethod->setDeclaration(analyzedNode);
    compiledMethod->setClosureSignature(resultType, argumentTypes);

    // Set the arguments declaration and definition node.
    for(size_t i = 0; i < analyzedNode->arguments.size(); ++i)
    {
        compiledMethod->setArgumentDeclarationNode(i, staticObjectCast<ASTArgumentDefinitionNode> (analyzedNode->arguments[i]));
        compiledMethod->setArgumentDefinitionNode(i, staticObjectCast<ASTArgumentDefinitionNode> (analyzedNode->arguments[i]));
    }

    ownerEntity->recordChildProgramEntityDefinition(compiledMethod);

    // Set the definition body.
    compiledMethod->setDefinition(analyzedNode, analyzedNode->body, environment);

    // Analyze the body.
    auto analyzedBody = compiledMethod->analyzeDefinitionWith(selfFromThis());
    if(analyzedBody->isASTErrorNode())
        return analyzedBody;

    analyzedNode->analyzedProgramEntity = compiledMethod;
    analyzedNode->analyzedType = compiledMethod->getFunctionalType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node)
{
    auto boundSymbol = node->binding ? node->binding : environment->lexicalScope->lookupSymbolRecursively(node->identifier);
    if(!boundSymbol)
    {
        if(environment->isLiteralArrayAnalysisEnvironment)
            return analyzeNodeIfNeededWithCurrentExpectedType(
                validAnyValue(node->identifier)->asASTNodeRequiredInPosition(node->sourcePosition)
            );

        return recordSemanticErrorInNode(node, formatString("Failed to find binding for identifier {0}.", {{node->identifier->printString()}}));
    }

    auto analyzedNode = shallowCloneObject(node);
    return boundSymbol->analyzeIdentifierReferenceNode(analyzedNode, selfFromThis());
}

AnyValuePtr ASTSemanticAnalyzer::visitCallNode(const ASTCallNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->function = analyzeNodeIfNeededWithTemporaryAutoType(analyzedNode->function);
    return analyzedNode->function->analyzedType->analyzeCallNode(analyzedNode, selfFromThis());
}

ASTNodePtr ASTSemanticAnalyzer::analyzeCallNodeWithFunctionalType(const ASTCallNodePtr &node, const FunctionalTypePtr &functionType)
{
    auto functionTypeArgumentCount = functionType->getArgumentCount();
    auto callArgumentCount = node->arguments.size();
    auto paramsType = functionType->getParamsType();
    auto hasCVarArg = false;
    if(paramsType)
    {
        hasCVarArg = paramsType->isCVarArgsType();
        --functionTypeArgumentCount;
        if(callArgumentCount < functionTypeArgumentCount)
        {
            // Analyze the arguments for discovering more error.
            for(auto &arg : node->arguments)
                analyzeNodeIfNeededWithTemporaryAutoType(arg);

            return recordSemanticErrorInNode(node, "Less arguments than required are supplied.");
        }
    }
    else if(callArgumentCount != functionTypeArgumentCount)
    {
        // Analyze the arguments for discovering more error.
        for(auto &arg : node->arguments)
            analyzeNodeIfNeededWithTemporaryAutoType(arg);

        return recordSemanticErrorInNode(node, "Call argument count mismatch.");
    }

    // Analyze the required arguments.
    for(size_t i = 0; i < functionTypeArgumentCount; ++i)
    {
        auto &arg = node->arguments[i];
        arg = analyzeNodeIfNeededWithExpectedType(arg, functionType->getArgument(i));
    }

    // Analyze the variadic arguments.
    if(paramsType)
    {
        auto paramUndecoratedType = paramsType->asUndecoratedType();
        if(hasCVarArg)
        {
            // CVarArg
            for(size_t i = functionTypeArgumentCount; i < callArgumentCount; ++i)
            {
                auto &arg = node->arguments[i];
                arg = analyzeNodeIfNeededWithExpectedType(arg, paramUndecoratedType);
            }
        }
        else
        {
            // Extract the variadic parameters.
            ASTNodePtrList variadicParameters;
            variadicParameters.reserve(callArgumentCount - functionTypeArgumentCount);
            for(size_t i = functionTypeArgumentCount; i < callArgumentCount; ++i)
                variadicParameters.push_back(node->arguments[i]);

            // Construct the variadic container.
            auto variadicContainerParam = basicMakeObject<ASTCallNode> ();
            if(variadicParameters.empty())
                variadicContainerParam->sourcePosition = node->sourcePosition;
            else
                variadicContainerParam->sourcePosition = variadicParameters.front()->sourcePosition;
            variadicContainerParam->function = paramUndecoratedType->asASTNodeRequiredInPosition(variadicContainerParam->sourcePosition);
            variadicContainerParam->arguments = variadicParameters;
            auto analyzedParams = analyzeNodeIfNeededWithExpectedType(variadicContainerParam, paramUndecoratedType);
            if(analyzedParams->isASTErrorNode())
                return analyzedParams;

            node->arguments.resize(functionTypeArgumentCount);
            node->arguments.push_back(analyzedParams);
        }
    }

    // Make sure there is no receiver type.
    if(!functionType->getReceiverType()->isVoidType())
        return recordSemanticErrorInNode(node, "Called method requires a receiver.");

    node->analyzedType = functionType->getResultType();
    return node;
}

AnyValuePtr ASTSemanticAnalyzer::visitLexicalScopeNode(const ASTLexicalScopeNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->analyzedScope = basicMakeObject<LexicalScope> ();
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
    auto result = shallowCloneObject(node);
    result->analyzedType = node->type;
    return result;
}

AnyValuePtr ASTSemanticAnalyzer::visitMakeAssociationNode(const ASTMakeAssociationNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->key = analyzeNodeIfNeededWithAutoType(analyzedNode->key);
    if(!analyzedNode->value)
        analyzedNode->value = getNilConstant()->asASTNodeRequiredInPosition(node->sourcePosition);
    analyzedNode->value = analyzeNodeIfNeededWithAutoType(analyzedNode->value);
    analyzedNode->analyzedType = LiteralAssociation::__staticType__();
    return optimizeAnalyzedMakeAssociationNode(analyzedNode);
}

ASTNodePtr ASTSemanticAnalyzer::optimizeAnalyzedMakeAssociationNode(const ASTMakeAssociationNodePtr &node)
{
    if(node->analyzedType == LiteralAssociation::__staticType__()
        && node->key->isASTLiteralValueNode()
        && node->value->isASTLiteralValueNode())
    {
        auto key = node->key.staticAs<ASTLiteralValueNode> ()->value;
        auto value = node->value.staticAs<ASTLiteralValueNode> ()->value;
        return analyzeNodeIfNeededWithCurrentExpectedType(
            LiteralAssociation::makeWith(key, value)->asASTNodeRequiredInPosition(node->sourcePosition)
        );
    }

    return node;
}
    
AnyValuePtr ASTSemanticAnalyzer::visitMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    auto expectedDictionaryType = LiteralDictionary::__staticType__();
    auto expectedAssociationType = LiteralAssociation::__staticType__();
    for(auto &element : analyzedNode->elements)
        element = analyzeNodeIfNeededWithExpectedType(element, expectedAssociationType);

    analyzedNode->analyzedType = expectedDictionaryType;
    return optimizeAnalyzedMakeDictionaryNode(analyzedNode);
}

ASTNodePtr ASTSemanticAnalyzer::optimizeAnalyzedMakeDictionaryNode(const ASTMakeDictionaryNodePtr &node)
{
    if(node->analyzedType != LiteralDictionary::__staticType__())
        return node;

    // Make sure all of the elements are composed of literal value.
    for(auto &el : node->elements)
    {
        if(!el->isASTLiteralValueNode())
            return node;
    }

    AnyValuePtrList elements;
    elements.reserve(node->elements.size());
    for(auto &el : node->elements)
        elements.push_back(staticObjectCast<ASTLiteralValueNode> (el)->value);

    return analyzeNodeIfNeededWithCurrentExpectedType(
        LiteralDictionary::makeFor(elements)->asASTNodeRequiredInPosition(node->sourcePosition)
    );
}

AnyValuePtr ASTSemanticAnalyzer::visitMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &node)
{
    auto literalEnvironment = environment;
    if(!environment->isLiteralArrayAnalysisEnvironment)
        literalEnvironment = literalEnvironment->languageSupport->createMakeLiteralArrayAnalysisEnvironment();

    return withEnvironmentDoAnalysis(literalEnvironment, [&](){
        auto analyzedNode = shallowCloneObject(node);
        auto hasError = false;
        ASTNodePtr errorNode;
        for(auto &el : analyzedNode->elements)
        {
            el = analyzeNodeIfNeededWithAutoType(el);
            if(el->isASTErrorNode())
            {
                if(!hasError)
                    errorNode = el;
                hasError = true;
            }
        }

        if(hasError)
            return errorNode;

        analyzedNode->analyzedType = LiteralArray::__staticType__();
        return optimizeAnalyzedMakeLiteralArrayNode(analyzedNode);
    });
}

ASTNodePtr ASTSemanticAnalyzer::optimizeAnalyzedMakeLiteralArrayNode(const ASTMakeLiteralArrayNodePtr &node)
{
    // Make sure that all of the elements are a literal value node.
    for(auto &el : node->elements)
    {
        if(!el->isASTLiteralValueNode())
            return node;
    }

    // Extract the literal values.
    AnyValuePtrList elements;
    elements.reserve(node->elements.size());
    for(auto &el : node->elements)
        elements.push_back(el.staticAs<ASTLiteralValueNode> ()->value);

    // Make a new literal value node with the literal elements. 
    return analyzeNodeIfNeededWithCurrentExpectedType(
        LiteralArray::makeFor(elements)->asASTNodeRequiredInPosition(node->sourcePosition)
    );
}

AnyValuePtr ASTSemanticAnalyzer::visitMakeAggregateNode(const ASTMakeAggregateNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

    // Evaluate the aggregate type.
    analyzedNode->aggregateType = evaluateTypeExpression(analyzedNode->aggregateType);
    if(analyzedNode->aggregateType->isASTErrorNode())
    {
        for(auto &el: node->elements)
            analyzeNodeIfNeededWithTemporaryAutoType(el);
        return analyzedNode->aggregateType;
    }

    auto aggregateType = unwrapTypeFromLiteralValue(analyzedNode->aggregateType);
    if(!aggregateType->isAggregateType())
        return recordSemanticErrorInNode(analyzedNode->aggregateType, "Expected an aggregate type.");

    const auto &layout = aggregateType.staticAs<AggregateType>()->getLayout();

    // Analyze the elements
    bool hasError = false;
    ASTNodePtr errorNode;
    for(size_t i = 0; i < analyzedNode->elements.size(); ++i)
    {
        auto &element = analyzedNode->elements[i];
        auto expectedElementType = layout->getTypeForNonPaddingSlot(int64_t(i));
        if(!expectedElementType)
            element = recordSemanticErrorInNode(element, "Excessive aggregate element.");
        else
            element = analyzeNodeIfNeededWithExpectedType(element, expectedElementType);

        if(element->isASTErrorNode())
        {
            if(!hasError)
                errorNode = element;
            hasError = true;
        }
    }

    if(hasError)
        return errorNode;

    analyzedNode->analyzedType = aggregateType;
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitMakeTupleNode(const ASTMakeTupleNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

    TypePtrList elementTypes;
    elementTypes.reserve(analyzedNode->elements.size());

    bool hasError = false;
    ASTNodePtr errorNode;
    for(auto &el : analyzedNode->elements)
    {
        el = analyzeNodeIfNeededWithAutoType(el);
        if(el->isASTErrorNode())
        {
            if(!hasError)
                errorNode = el;
            hasError = true;
        }

        elementTypes.push_back(el->analyzedType);
    }

    if(hasError)
        return errorNode;

    analyzedNode->analyzedType = TupleType::make(elementTypes);
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitMakeVariantNode(const ASTMakeVariantNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->value = analyzeNodeIfNeededWithAutoType(analyzedNode->value);
    analyzedNode->variantType = evaluateTypeExpression(analyzedNode->variantType);

    if(analyzedNode->value->isASTErrorNode())
        return analyzedNode->value;

    if(analyzedNode->variantType->isASTErrorNode())
        return analyzedNode->variantType;

    auto variantType = unwrapTypeFromLiteralValue(analyzedNode->variantType);
    if(!variantType->isVariantType())
        return recordSemanticErrorInNode(analyzedNode->variantType, "Expected a variant type.");

    auto elementType = analyzedNode->value->analyzedType->asDecayedType();
    auto typeSelectorIndex = variantType.staticAs<VariantType> ()->findTypeSelectorIndexFor(elementType);
    if(!typeSelectorIndex.has_value())
        return recordSemanticErrorInNode(analyzedNode, formatString("Invalid source type {0} for constructing variant type {1}.", {elementType->printString(), variantType->printString()}));

    analyzedNode->typeSelectorIndex = typeSelectorIndex.value();
    analyzedNode->analyzedType = variantType;
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitMessageChainNode(const ASTMessageChainNodePtr &node)
{
    // Validate the chained message nodes.
    for(auto &message : node->messages)
    {
        if(!message->isASTMessageChainMessageNode())
            return recordSemanticErrorInNode(message, "Expected a chained message AST node.");
    }

    // Single message degenerate case.
    if(node->messages.size() == 1)
        return staticObjectCast<ASTMessageChainMessageNode> (node->messages[0])->asMessageSendNodeWithReceiver(node->receiver);

    // Turn into a sequence of receiver-less messages.
    if(!node->receiver)
    {
        auto result = basicMakeObject<ASTSequenceNode> ();
        result->sourcePosition = node->sourcePosition;
        result->expressions.reserve(node->messages.size());
        for(auto &message : node->messages)
        {
            auto convertedMessage = message.staticAs<ASTMessageChainMessageNode> ()->asMessageSendNodeWithReceiver(nullptr);
            result->expressions.push_back(convertedMessage);
        }
        return analyzeNodeIfNeededWithCurrentExpectedType(result);
    }

    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->receiver = analyzeNodeIfNeededWithTemporaryAutoType(analyzedNode->receiver);

    // Support inline analysis of meta-builders.
    if(analyzedNode->receiver->isASTLiteralValueNode() && analyzedNode->receiver->analyzedType->supportsMessageAnalysisByLiteralValueReceivers())
    {
        auto result = analyzedNode->receiver;
        for(auto &message : analyzedNode->messages)
        {
            auto convertedMessage = staticObjectCast<ASTMessageChainMessageNode> (message)->asMessageSendNodeWithReceiver(analyzedNode->receiver);
            result = analyzeNodeIfNeededWithTemporaryAutoType(convertedMessage);
        }
        return result;
    }

    // Expand into a sequence with a variable
    auto result = basicMakeObject<ASTSequenceNode> ();
    result->sourcePosition = node->sourcePosition;
    result->expressions.reserve(1 + node->messages.size());

    // Make the receiver variable name;
    auto receiverVariableName = basicMakeObject<MessageChainReceiverName> ();
    receiverVariableName->sourcePosition = node->sourcePosition;

    // Create a receiver variable.
    auto receiverVariable = basicMakeObject<ASTLocalVariableNode> ();
    receiverVariable->name = receiverVariableName->asASTNodeRequiredInPosition(node->sourcePosition);
    receiverVariable->initialValue = node->receiver;
    receiverVariable->typeInferenceMode = TypeInferenceMode::TemporaryReference;
    result->expressions.push_back(receiverVariable);

    // Create the receiver identifier.
    auto receiverIdentifier = basicMakeObject<ASTIdentifierReferenceNode> ();
    receiverIdentifier->sourcePosition = node->sourcePosition;
    receiverIdentifier->identifier = receiverVariableName;

    // Convert the chained messages.
    for(auto &message : analyzedNode->messages)
    {
        auto convertedMessage = message.staticAs<ASTMessageChainMessageNode> ()->asMessageSendNodeWithReceiver(receiverIdentifier);
        result->expressions.push_back(convertedMessage);
    }

    return analyzeNodeIfNeededWithCurrentExpectedType(result);
}

AnyValuePtr ASTSemanticAnalyzer::visitMessageSendNode(const ASTMessageSendNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->selector = analyzeNodeIfNeededWithAutoType(analyzedNode->selector);

    if(analyzedNode->receiver)
    {
        analyzedNode->receiver = analyzeNodeIfNeededWithTemporaryAutoType(analyzedNode->receiver);

        // Delegate to the receiver type.
        return analyzedNode->receiver->analyzedType->analyzeMessageSendNode(analyzedNode, selfFromThis());
    }
    else
    {
        if(!analyzedNode->selector->isASTLiteralValueNode())
            return recordSemanticErrorInNode(node, "Cannot analyze message send without receiver that has a non-literal selector.");

        auto literalSelectorNode = staticObjectCast<ASTLiteralValueNode> (analyzedNode->selector);
        auto literalSelector = literalSelectorNode->value;
        auto boundSymbol = environment->lexicalScope->lookupSymbolRecursively(literalSelectorNode->value);
        if(!boundSymbol)
            return recordSemanticErrorInNode(node, formatString("Failed to find a definition for a message without receiver using the {0} selector.", {{literalSelector->printString()}}));

        return boundSymbol->analyzeMessageSendNode(analyzedNode, selfFromThis());
    }
}

AnyValuePtr ASTSemanticAnalyzer::visitParseErrorNode(const ASTParseErrorNodePtr &node)
{
    auto result = shallowCloneObject(node);
    result->analyzedType = Type::getCompilationErrorValueType();
    recordedErrors.push_back(result);
    return result;
}

AnyValuePtr ASTSemanticAnalyzer::visitPragmaNode(const ASTPragmaNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->selector = analyzeNodeIfNeededWithExpectedType(analyzedNode->selector, Type::getLiteralSymbolValue());
    for(auto &argument : node->arguments)
        argument = analyzeNodeIfNeededWithExpectedType(argument, Type::getLiteralValueType());

    analyzedNode->analyzedType = Type::getPragmaType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitQuasiQuoteNode(const ASTQuasiQuoteNodePtr &node)
{
    auto quasiQuoteAnalyzer = basicMakeObject<ASTQuasiQuoteAnalyzer> ();
    quasiQuoteAnalyzer->analyzer = selfFromThis();
    return quasiQuoteAnalyzer->analyzeQuasiQuote(node);
}

AnyValuePtr ASTSemanticAnalyzer::visitQuasiUnquoteNode(const ASTQuasiUnquoteNodePtr &node)
{
    return recordSemanticErrorInNode(node, "Invalid location for a quasi-unquote expression");
}

AnyValuePtr ASTSemanticAnalyzer::visitQuoteNode(const ASTQuoteNodePtr &node)
{
    auto result = shallowCloneObject(node);
    result->analyzedType = ASTNode::__staticType__();
    return result;
}

AnyValuePtr ASTSemanticAnalyzer::visitSequenceNode(const ASTSequenceNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

    // Simplify the single element sequence.
    if(analyzedNode->pragmas.empty() && analyzedNode->expressions.size() == 1)
        return analyzeNodeIfNeededWithTypeInference(analyzedNode->expressions[0], currentExpectedType, true);

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
        bool hasControlFlowEscapeType = false;
        TypePtr controlFlowEscapeType;

        for(size_t i = 0; i < expressionCount; ++i)
        {
            auto isLast = i + 1 >= expressionCount;
            auto &expression = analyzedNode->expressions[i];

            if(isLast && !hasControlFlowEscapeType)
                expression = analyzeNodeIfNeededWithTypeInference(expression, currentExpectedType, true);
            else
                expression = analyzeNodeIfNeededWithExpectedType(expression, voidType, true);

            if(!hasControlFlowEscapeType && expression->analyzedType->isControlFlowEscapeType())
            {
                hasControlFlowEscapeType = true;
                controlFlowEscapeType = expression->analyzedType;
            }
        }

        analyzedNode->analyzedType = hasControlFlowEscapeType ? controlFlowEscapeType : analyzedNode->expressions.back()->analyzedType;
    }

    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitSpliceNode(const ASTSpliceNodePtr &node)
{
    return recordSemanticErrorInNode(node, "Invalid location for a splice expression");
}

AnyValuePtr ASTSemanticAnalyzer::visitSemanticErrorNode(const ASTSemanticErrorNodePtr &node)
{
    auto result = shallowCloneObject(node);
    result->analyzedType = Type::getCompilationErrorValueType();
    recordedErrors.push_back(result);
    return result;
}

AnyValuePtr ASTSemanticAnalyzer::visitLocalVariableNode(const ASTLocalVariableNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

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
            analyzedNode->initialValue = analyzeNodeIfNeededWithAutoTypeInferenceMode(analyzedNode->initialValue, analyzedNode->typeInferenceMode, analyzedNode->isMutable, true);
            analyzedNode->type = analyzedNode->initialValue->analyzedType->asASTNodeRequiredInPosition(analyzedNode->sourcePosition);
        }
        else
        {
            sysmelAssert("TODO: Support deferred and/or default type " && false);    
        }
    }

    auto valueType = Type::getUndefinedType();
    if(analyzedNode->type->isASTLiteralValueNode())
    {
        auto literalTypeNode = staticObjectCast<ASTLiteralValueNode> (analyzedNode->type);
        sysmelAssert(literalTypeNode->value->isType());

        valueType = staticObjectCast<Type> (literalTypeNode->value);
    }

    // Make sure the initial value is analyzed.
    if(analyzedNode->initialValue)
        analyzedNode->initialValue = analyzeNodeIfNeededWithExpectedType(analyzedNode->initialValue, valueType, true);

    // Create the local variable.
    auto localVariable = basicMakeObject<LocalVariable> ();
    localVariable->setDefinitionParameters(name, valueType, analyzedNode->isMutable);
    localVariable->setDeclarationNode(analyzedNode);
    localVariable->setDefinitionNode(analyzedNode);

    // Record the program entity.
    environment->localDefinitionsOwner->recordChildProgramEntityDefinition(localVariable);

    // Bind the local variable in the current lexical scope.
    environment->lexicalScope->setSymbolBinding(name, localVariable);

    // Set it in the analyzed node.
    analyzedNode->analyzedType = localVariable->getReferenceType();
    analyzedNode->analyzedProgramEntity = localVariable;
    return analyzedNode;
}


AnyValuePtr ASTSemanticAnalyzer::visitGlobalVariableNode(const ASTGlobalVariableNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    
    // Concretize the default visibility.
    if(analyzedNode->visibility == ProgramEntityVisibility::Default)
        analyzedNode->visibility = ProgramEntityVisibility::Internal;

    auto name = evaluateNameSymbolValue(analyzedNode->name);
    auto ownerEntity = environment->programEntityForPublicDefinitions;

    GlobalVariablePtr globalVariable;

    // Check the name.
    if(name)
    {
        // Forbid reserved names.
        if(isNameReserved(name))
            return recordSemanticErrorInNode(analyzedNode, formatString("Global variable {1} overrides reserved name.", {{name->printString()}}));

        auto boundSymbol = ownerEntity->lookupLocalSymbolFromScope(name, environment->lexicalScope);
        if(boundSymbol)
        {
            if(!boundSymbol->isGlobalVariable())
                return recordSemanticErrorInNode(analyzedNode, formatString("Global variable {1} overrides previous non-global variable definition in its parent program entity ({2}).",
                    {{name->printString(), boundSymbol->printString()}}));

            globalVariable = staticObjectCast<GlobalVariable> (boundSymbol);
        }
    }

    if(analyzedNode->type)
    {
        analyzedNode->type = evaluateTypeExpression(analyzedNode->type);
    }
    else
    {
        if(analyzedNode->initialValue)
        {
            analyzedNode->initialValue = analyzeNodeIfNeededWithAutoTypeInferenceMode(analyzedNode->initialValue, analyzedNode->typeInferenceMode, analyzedNode->isMutable, true);
            analyzedNode->type = analyzedNode->initialValue->analyzedType->asASTNodeRequiredInPosition(analyzedNode->sourcePosition);
        }
        else
        {
            sysmelAssert("TODO: Support deferred and/or default type " && false);    
        }
    }

    auto valueType = Type::getUndefinedType();
    if(analyzedNode->type->isASTLiteralValueNode())
    {
        auto literalTypeNode = staticObjectCast<ASTLiteralValueNode> (analyzedNode->type);
        sysmelAssert(literalTypeNode->value->isType());

        valueType = staticObjectCast<Type> (literalTypeNode->value);
    }

    // Make sure the initial value is analyzed.
    if(analyzedNode->initialValue)
        analyzedNode->initialValue = analyzeNodeIfNeededWithExpectedType(analyzedNode->initialValue, valueType, true);

    // Create the global variable.
    if(!globalVariable)
    {
        globalVariable = basicMakeObject<GlobalVariable> ();
        globalVariable->setVisibility(node->visibility);
        globalVariable->setExternalLanguageMode(node->externalLanguageMode);
        globalVariable->setDllLinkageMode(node->dllLinkageMode);
        globalVariable->setDefinitionParameters(name, valueType, analyzedNode->isMutable);
        globalVariable->setDeclarationNode(analyzedNode);
        globalVariable->setDefinitionNode(analyzedNode);
        globalVariable->enqueuePendingSemanticAnalysis();

        ownerEntity->recordChildProgramEntityDefinition(globalVariable);
        ownerEntity->bindProgramEntityWithVisibility(globalVariable, analyzedNode->visibility);
    }

    if(analyzedNode->initialValue)
    {
        if(globalVariable->initialValueCodeFragment)
            return recordSemanticErrorInNode(analyzedNode, formatString("Global variable {1} has multiple definitions for its initial value.", {{name->printString()}}));

        globalVariable->initialValueCodeFragment = DeferredCompileTimeCodeFragment::make(analyzedNode->initialValue, environment);
        globalVariable->setDefinitionNode(analyzedNode);
    }

    // Set it in the analyzed node.
    analyzedNode->analyzedType = globalVariable->getReferenceType();
    analyzedNode->analyzedProgramEntity = globalVariable;
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitFieldVariableNode(const ASTFieldVariableNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->isMutable = true;
    
    // Concretize the default visibility.
    if(analyzedNode->visibility == ProgramEntityVisibility::Default)
        analyzedNode->visibility = ProgramEntityVisibility::Internal;

    auto name = evaluateNameSymbolValue(analyzedNode->name);
    auto ownerEntity = environment->programEntityForPublicDefinitions;

    if(!ownerEntity->canHaveUserDefinedFields())
        return recordSemanticErrorInNode(analyzedNode, formatString("Fields cannot be defined inside of program entity {0}.", {{ownerEntity->printString()}}));

    FieldVariablePtr fieldVariable;

    // Check the name.
    if(name)
    {
        // Forbid reserved names.
        if(isNameReserved(name))
            return recordSemanticErrorInNode(analyzedNode, formatString("Field {0} overrides reserved name.", {{name->printString()}}));

        auto boundSymbol = ownerEntity->lookupLocalSymbolFromScope(name, environment->lexicalScope);
        if(boundSymbol)
            return recordSemanticErrorInNode(analyzedNode, formatString("Field {0} overrides previous definition in its parent program entity ({1}).",
                    {{name->printString(), boundSymbol->printString()}}));
    }

    if(analyzedNode->type)
    {
        analyzedNode->type = evaluateTypeExpression(analyzedNode->type);
    }
    else
    {
        if(analyzedNode->initialValue)
        {
            analyzedNode->initialValue = analyzeNodeIfNeededWithAutoTypeInferenceMode(analyzedNode->initialValue, analyzedNode->typeInferenceMode, analyzedNode->isMutable, true);
            analyzedNode->type = analyzedNode->initialValue->analyzedType->asASTNodeRequiredInPosition(analyzedNode->sourcePosition);
        }
        else
        {
            sysmelAssert("TODO: Support deferred and/or default type " && false);    
        }
    }

    auto valueType = Type::getUndefinedType();
    if(analyzedNode->type->isASTLiteralValueNode())
    {
        auto literalTypeNode = staticObjectCast<ASTLiteralValueNode> (analyzedNode->type);
        sysmelAssert(literalTypeNode->value->isType());

        valueType = staticObjectCast<Type> (literalTypeNode->value);
    }

    // Make sure the initial value is analyzed.
    if(analyzedNode->initialValue)
        analyzedNode->initialValue = analyzeNodeIfNeededWithExpectedType(analyzedNode->initialValue, valueType, true);

    // Create the global variable.
    if(!fieldVariable)
    {
        fieldVariable = basicMakeObject<FieldVariable> ();
        fieldVariable->setDefinitionParameters(name, valueType, analyzedNode->isMutable);
        fieldVariable->setDeclarationNode(analyzedNode);
        fieldVariable->setDefinitionNode(analyzedNode);
        fieldVariable->enqueuePendingSemanticAnalysis();

        ownerEntity->addFieldVariableWithVisibility(fieldVariable, analyzedNode->visibility);
    }

    if(analyzedNode->initialValue)
    {
        if(fieldVariable->initialValueCodeFragment)
            return recordSemanticErrorInNode(analyzedNode, formatString("Global variable {0} has multiple definitions for its initial value.", {{name->printString()}}));

        fieldVariable->initialValueCodeFragment = DeferredCompileTimeCodeFragment::make(analyzedNode->initialValue, environment);
    }

    // Set it in the analyzed node.
    analyzedNode->analyzedType = FieldVariable::__staticType__();
    analyzedNode->analyzedProgramEntity = fieldVariable;
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitCompileTimeConstantNode(const ASTCompileTimeConstantNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

    // Concretize the default visibility.
    if(analyzedNode->visibility == ProgramEntityVisibility::Default)
        analyzedNode->visibility = ProgramEntityVisibility::Internal;

    auto name = evaluateNameSymbolValue(analyzedNode->name);
    auto ownerEntity = environment->programEntityForPublicDefinitions;

    CompileTimeConstantPtr compileTimeConstant;

    // Check the name.
    if(name)
    {
        // Forbid reserved names.
        if(isNameReserved(name))
            return recordSemanticErrorInNode(analyzedNode, formatString("Compile time constant {1} overrides reserved name.", {{name->printString()}}));

        auto boundSymbol = ownerEntity->lookupLocalSymbolFromScope(name, environment->lexicalScope);
        if(boundSymbol)
        {
            if(!boundSymbol->isCompileTimeConstant())
                return recordSemanticErrorInNode(analyzedNode, formatString("Compile time constant {1} overrides previous non-compile time constant definition in its parent program entity ({2}).",
                    {{name->printString(), boundSymbol->printString()}}));

            compileTimeConstant = staticObjectCast<CompileTimeConstant> (boundSymbol);
        }
    }

    TypePtr constantType;
    if(analyzedNode->type)
    {
        analyzedNode->type = evaluateTypeExpression(analyzedNode->type);
        if(analyzedNode->type->isASTLiteralTypeNode())
            constantType = unwrapTypeFromLiteralValue(analyzedNode->type);
    }

    // Create the global variable.
    if(!compileTimeConstant)
    {
        compileTimeConstant = basicMakeObject<CompileTimeConstant> ();
        compileTimeConstant->name = name;
        compileTimeConstant->enqueuePendingSemanticAnalysis();

        ownerEntity->recordChildProgramEntityDefinition(compileTimeConstant);
        ownerEntity->bindProgramEntityWithVisibility(compileTimeConstant, analyzedNode->visibility);
    }

    if(constantType)
        compileTimeConstant->valueType = constantType;

    if(analyzedNode->value)
    {
        if(compileTimeConstant->valueCodeFragment)
            return recordSemanticErrorInNode(analyzedNode, formatString("Compile time constant variable {1} has multiple definitions for its value.", {{name->printString()}}));

        compileTimeConstant->valueCodeFragment = DeferredCompileTimeCodeFragment::make(analyzedNode->value, environment);
    }

    analyzedNode->analyzedProgramEntity = compileTimeConstant;
    analyzedNode->analyzedType = CompileTimeConstant::__staticType__();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitFieldVariableAccessNode(const ASTFieldVariableAccessNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->aggregate = analyzeNodeIfNeededWithTemporaryAutoType(analyzedNode->aggregate);
    if(analyzedNode->aggregate->isASTErrorNode())
        return analyzedNode->aggregate;

    analyzedNode->analyzedType = analyzedNode->fieldVariable->getReferenceType();
    if(analyzedNode->aggregate->analyzedType->isConstOrConstReferenceType())
        analyzedNode->analyzedType = analyzedNode->analyzedType->asConstOrConstReferenceType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitSlotAccessNode(const ASTSlotAccessNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->aggregate = analyzeNodeIfNeededWithTemporaryAutoType(analyzedNode->aggregate);
    if(analyzedNode->aggregate->isASTErrorNode())
        return analyzedNode->aggregate;

    analyzedNode->analyzedType = node->slotReferenceType;
    return analyzedNode;   
}

AnyValuePtr ASTSemanticAnalyzer::visitVariableAccessNode(const ASTVariableAccessNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    const auto &variable = analyzedNode->variable;
    if(variable->isFunctionVariable())
    {
        auto variableOwner = variable->getParentProgramEntity();
        sysmelAssert(variableOwner && variableOwner->isCompiledMethod());
        sysmelAssert(environment->localDefinitionsOwner && environment->localDefinitionsOwner->isCompiledMethod());
        if(variableOwner != environment->localDefinitionsOwner)
            environment->localDefinitionsOwner.staticAs<CompiledMethod> ()->recordCapturedFunctionVariable(staticObjectCast<FunctionVariable> (variable));
    }

    analyzedNode->analyzedType = variable->getReferenceType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitFunctionNode(const ASTFunctionNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

    // Concretize the default visibility.
    if(analyzedNode->visibility == ProgramEntityVisibility::Default)
        analyzedNode->visibility = ProgramEntityVisibility::LexicalScope;

    // TODO: Use the current expected type to assist the function type inference.

    // Analyze the arguments node.
    bool hasError = false;
    for(size_t i = 0; i < analyzedNode->arguments.size(); ++i)
    {
        auto &arg = analyzedNode->arguments[i];
        sysmelAssert(arg->isASTArgumentDefinitionNode());
        arg = analyzeArgumentDefinitionNodeWithExpectedType(staticObjectCast<ASTArgumentDefinitionNode> (arg), currentExpectedType->getExpectedFunctionalArgumentType(i));
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
    bool isLocalDefinition = analyzedNode->isRegisteredInLexicalScope();
    auto ownerEntity = isLocalDefinition ? environment->localDefinitionsOwner : environment->programEntityForPublicDefinitions;

    // Check the name.
    if(name)
    {
        // Forbid reserved names.
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
                    return recordSemanticErrorInNode(analyzedNode, formatString("Function {1} overrides previous non-function definition in its parent program entity ({2}).",
                        {{name->printString(), boundSymbol->printString()}}));

                compiledMethod = staticObjectCast<CompiledMethod> (boundSymbol);
                if((compiledMethod->getMethodFlags() & analyzedNode->methodFlags) != analyzedNode->methodFlags)
                    return recordSemanticErrorInNode(analyzedNode, formatString("Function {1} has conflicting method flags with previous definition ({2}).",
                        {{name->printString(), compiledMethod->printString()}}));
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
        compiledMethod = basicMakeObject<CompiledMethod> ();
        compiledMethod->setName(name);
        compiledMethod->setDeclaration(analyzedNode);
        compiledMethod->setVisibility(node->visibility);
        compiledMethod->setExternalLanguageMode(node->externalLanguageMode);
        compiledMethod->setDllLinkageMode(node->dllLinkageMode);

        // If this is a local definition, then define it as a closure.
        if(isLocalDefinition)
            compiledMethod->setClosureSignature(resultType, argumentTypes);
        else
            compiledMethod->setFunctionSignature(resultType, argumentTypes);

        // Set the arguments declaration node.
        for(size_t i = 0; i < analyzedNode->arguments.size(); ++i)
            compiledMethod->setArgumentDeclarationNode(i, staticObjectCast<ASTArgumentDefinitionNode> (analyzedNode->arguments[i]));
        compiledMethod->addMethodFlags(analyzedNode->methodFlags);
        ownerEntity->recordChildProgramEntityDefinition(compiledMethod);

        if(isLocalDefinition)
            environment->lexicalScope->setSymbolBinding(name, compiledMethod);
        else
            ownerEntity->bindProgramEntityWithVisibility(compiledMethod, analyzedNode->visibility);
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
            compiledMethod->setArgumentDefinitionNode(i, staticObjectCast<ASTArgumentDefinitionNode> (analyzedNode->arguments[i]));

        // Set the definition body.
        compiledMethod->setDefinition(analyzedNode, analyzedNode->body, environment);
        compiledMethod->enqueuePendingSemanticAnalysis();
    }

    // If this is a local definition, analyze in place.
    if(isLocalDefinition)
    {
        auto analyzedBody = compiledMethod->analyzeDefinitionWith(selfFromThis());
        if(analyzedBody->isASTErrorNode())
            return analyzedBody;
    }

    analyzedNode->analyzedProgramEntity = compiledMethod;
    analyzedNode->analyzedType = compiledMethod->getFunctionalType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitTemplateNode(const ASTTemplateNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

    // Concretize the default visibility.
    if(analyzedNode->visibility == ProgramEntityVisibility::Default)
        analyzedNode->visibility = ProgramEntityVisibility::Internal;

    // Analyze the arguments node.
    bool hasError = false;
    for(size_t i = 0; i < analyzedNode->arguments.size(); ++i)
    {
        auto &arg = analyzedNode->arguments[i];
        sysmelAssert(arg->isASTArgumentDefinitionNode());
        arg = analyzeTemplateArgumentDefinitionNode(staticObjectCast<ASTArgumentDefinitionNode> (arg));
        hasError = hasError || arg->isASTErrorNode();
    }

    // Evaluate the name.
    auto name = evaluateNameSymbolValue(analyzedNode->name);

    TemplatePtr templateEntity;
    bool alreadyRegistered = false;
    auto ownerEntity = environment->programEntityForPublicDefinitions;

    // Check the name.
    if(name)
    {
        // Forbid reserved names.
        if(isNameReserved(name))
            return recordSemanticErrorInNode(analyzedNode, formatString("Template {0} overrides reserved name.", {{name->printString()}}));

        auto boundSymbol = ownerEntity->lookupLocalSymbolFromScope(name, environment->lexicalScope);
        if(boundSymbol)
        {
            if(!boundSymbol->isTemplate())
                return recordSemanticErrorInNode(analyzedNode, formatString("Template {0} overrides previous non-namespace definition in its parent program entity ({1}).",
                    {{name->printString(), boundSymbol->printString()}}));

            templateEntity = staticObjectCast<Template> (boundSymbol);
            alreadyRegistered = true;
        }
    }

    // Abort the remaining of the template error.
    if(hasError)
    {
        analyzedNode->analyzedType = Type::getCompilationErrorValueType();
        return analyzedNode;
    }

    // Extract the argumnet types.
    TypePtrList argumentTypes;
    argumentTypes.reserve(analyzedNode->arguments.size());
    for(const auto &arg : analyzedNode->arguments)
        argumentTypes.push_back(arg->analyzedType);

    // Create the template if missing.
    if(!templateEntity)
    {
        templateEntity = basicMakeObject<Template> ();
        templateEntity->setName(name);
        templateEntity->setDeclarationNode(analyzedNode);
        templateEntity->setArgumentTypes(argumentTypes);
        templateEntity->setVisibility(node->visibility);
        templateEntity->setExternalLanguageMode(node->externalLanguageMode);
        templateEntity->setDllLinkageMode(node->dllLinkageMode);
        templateEntity->enqueuePendingSemanticAnalysis();

        // Set the arguments declaration node.
        for(size_t i = 0; i < analyzedNode->arguments.size(); ++i)
            templateEntity->setArgumentDeclarationNode(i, staticObjectCast<ASTArgumentDefinitionNode> (analyzedNode->arguments[i]));
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
            templateEntity->setArgumentDefinitionNode(i, staticObjectCast<ASTArgumentDefinitionNode> (analyzedNode->arguments[i]));

        // Add the definition body.
        templateEntity->addDefinition(analyzedNode, analyzedNode->body, environment);
    }

    // Register the template.
    if(name)
    {
        if(!alreadyRegistered)
        {
            ownerEntity->recordChildProgramEntityDefinition(templateEntity);
            ownerEntity->bindProgramEntityWithVisibility(templateEntity, analyzedNode->visibility);
        }
    }

    analyzedNode->analyzedProgramEntity = templateEntity;
    analyzedNode->analyzedType = Template::__staticType__();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitMethodNode(const ASTMethodNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

    // Concretize the default visibility.
    if(analyzedNode->visibility == ProgramEntityVisibility::Default)
        analyzedNode->visibility = ProgramEntityVisibility::Public;
    bool isMacro = (analyzedNode->methodFlags & MethodFlags::Macro) != MethodFlags::None;
    bool isFallback = (analyzedNode->methodFlags & MethodFlags::Fallback) != MethodFlags::None;
    bool hasVirtualSend = (analyzedNode->methodFlags & MethodFlags::VirtualSendFlags) != MethodFlags::None;
    bool isOverride = (analyzedNode->methodFlags & MethodFlags::Override) != MethodFlags::None;
    bool isStatic = (analyzedNode->methodFlags & MethodFlags::Static) != MethodFlags::None;
    bool hasConstReceiver = (analyzedNode->methodFlags & MethodFlags::Const) != MethodFlags::None;

    // Analyze the arguments node.
    bool hasError = false;
    for(size_t i = 0; i < analyzedNode->arguments.size(); ++i)
    {
        auto &arg = analyzedNode->arguments[i];
        if(arg->isASTArgumentDefinitionNode())
        {
            arg = analyzeArgumentDefinitionNodeWithExpectedType(staticObjectCast<ASTArgumentDefinitionNode> (arg), currentExpectedType->getExpectedFunctionalArgumentType(i), isMacro);
        }
        else
        {
            if(!arg->isASTErrorNode())
                arg = recordSemanticErrorInNode(arg, "Expected an argument definition.");
        }
        
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
        else if(isMacro || environment->defaultResultType)
        {
            auto defaultResultType = isMacro ? ASTNode::__staticType__() : environment->defaultResultType;
            analyzedNode->resultType = defaultResultType->asASTNodeRequiredInPosition(analyzedNode->sourcePosition);
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
    if(!name)
        return recordSemanticErrorInNode(analyzedNode, "Anonymous methods are not supported.");

    // Make sure that fallback is used in conjunction with the macro flag.
    if(isFallback && !isMacro)
        return recordSemanticErrorInNode(analyzedNode, "Non macro fallback methods are not supported.");

    CompiledMethodPtr compiledMethod;
    bool alreadyRegistered = false;
    auto ownerEntity = environment->programEntityForPublicDefinitions;
    TypePtr receiverType;
    if(isMacro)
    {
        receiverType = MacroInvocationContext::__staticType__();
    }
    else
    {
        if(isStatic)
            receiverType = Type::getVoidType();
        else if((analyzedNode->methodFlags & MethodFlags::Const) != MethodFlags::None)
            receiverType = ownerEntity->asConstReceiverType();
        else
            receiverType = ownerEntity->asReceiverType();
    }

    // Forbid reserved names.
    if(isNameReserved(name))
        return recordSemanticErrorInNode(analyzedNode, formatString("Method {1} overrides reserved name.", {{name->printString()}}));

    // Check for a previous method with the same selector.
    auto previousMethod = ownerEntity->lookupLocalSelector(name);
    if(previousMethod)
    {
        if(!previousMethod->isCompiledMethod())
            return recordSemanticErrorInNode(analyzedNode, formatString("Method {1} overrides previous non-method definition in its parent program entity ({2}).",
                {{name->printString(), previousMethod->printString()}}));

        compiledMethod = staticObjectCast<CompiledMethod> (previousMethod);
        if((compiledMethod->getMethodFlags() & analyzedNode->methodFlags) != analyzedNode->methodFlags)
            return recordSemanticErrorInNode(analyzedNode, formatString("Method {1} has conflicting method flags with previous definition ({2}).",
                {{name->printString(), previousMethod->printString()}}));
        alreadyRegistered = true;
    }

    // Abort the remaining analysis of the method in case of error.
    if(hasError)
    {
        analyzedNode->analyzedType = Type::getCompilationErrorValueType();
        return analyzedNode;
    }

    // Static vs Virtuality
    if(isStatic && hasVirtualSend)
        return recordSemanticErrorInNode(analyzedNode, "Static cannot be used in combination of abstract, virtual or override.");
    else if(isMacro && hasVirtualSend)
        return recordSemanticErrorInNode(analyzedNode, "Macros cannot virtual send semantics.");
    else if(!ownerEntity->canHaveVirtualMethods() && hasVirtualSend)
        return recordSemanticErrorInNode(analyzedNode, formatString("{0} cannot have virtual methods.", {ownerEntity->printString()}));

    // Create the function type.
    TypePtrList argumentTypes;
    argumentTypes.reserve(analyzedNode->arguments.size());
    for(const auto &arg : analyzedNode->arguments)
        argumentTypes.push_back(arg->analyzedType);
    auto resultType = unwrapTypeFromLiteralValue(analyzedNode->resultType);

    // Find the parent method and check the virtuality semantics.
    auto parentMethod = ownerEntity->lookupParentOverridenMethod(name, hasConstReceiver, argumentTypes);
    if(parentMethod && parentMethod->hasVirtualSendSemantics() && !isOverride)
        return recordSemanticErrorInNode(analyzedNode, formatString("Override is required for this method", {ownerEntity->printString()}));
    
    // Create the compiled method if missing.
    if(!compiledMethod)
    {
        compiledMethod = basicMakeObject<CompiledMethod> ();
        compiledMethod->setName(name);
        compiledMethod->setDeclaration(analyzedNode);
        compiledMethod->setMethodSignature(receiverType, resultType, argumentTypes);
        compiledMethod->setVisibility(node->visibility);
        compiledMethod->setExternalLanguageMode(node->externalLanguageMode);
        compiledMethod->setDllLinkageMode(node->dllLinkageMode);
        compiledMethod->setOverridenParentMethod(parentMethod);

        // Set the arguments declaration node.
        for(size_t i = 0; i < analyzedNode->arguments.size(); ++i)
            compiledMethod->setArgumentDeclarationNode(i, staticObjectCast<ASTArgumentDefinitionNode> (analyzedNode->arguments[i]));
        compiledMethod->addMethodFlags(analyzedNode->methodFlags);
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
            compiledMethod->setArgumentDefinitionNode(i, staticObjectCast<ASTArgumentDefinitionNode> (analyzedNode->arguments[i]));

        // Set the definition body.
        compiledMethod->setDefinition(analyzedNode, analyzedNode->body, environment);
        compiledMethod->enqueuePendingSemanticAnalysis();
    }

    // Register the compiled method.
    if(!alreadyRegistered)
    {
        ownerEntity->recordChildProgramEntityDefinition(compiledMethod);
        if(isMacro)
        {
            if(isFallback)
                ownerEntity->addMacroFallbackMethodWithSelector(compiledMethod, name);
            else
                ownerEntity->addMacroMethodWithSelector(compiledMethod, name);
        }
        else
        {
            if(hasVirtualSend)
                ownerEntity->addVirtualMethod(compiledMethod);
            ownerEntity->addMethodWithSelector(compiledMethod, name);
        }
    }

    analyzedNode->analyzedProgramEntity = compiledMethod;
    analyzedNode->analyzedType = compiledMethod->getFunctionalType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitNamespaceNode(const ASTNamespaceNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

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

                namespaceEntity = staticObjectCast<Namespace> (boundSymbol);
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
            ownerEntity->bindProgramEntityWithVisibility(namespaceEntity, ProgramEntityVisibility::Public);
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

AnyValuePtr ASTSemanticAnalyzer::visitEnumNode(const ASTEnumNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

    auto name = evaluateNameSymbolValue(analyzedNode->name);
    auto ownerEntity = environment->programEntityForPublicDefinitions;

    EnumTypePtr enumType;
    if(name)
    {
        if(isNameReserved(name))
            return recordSemanticErrorInNode(analyzedNode, formatString("Class {1} overrides reserved name.",
                    {{name->printString()}}));

        // Find an existing class with the same name.
        {
            auto boundSymbol = ownerEntity->lookupLocalSymbolFromScope(name, environment->lexicalScope);
            if(boundSymbol)
            {
                if(!boundSymbol->isEnumType())
                    return recordSemanticErrorInNode(analyzedNode, formatString("Class ({1}) overrides previous non-class definition in its parent program entity ({2}).",
                        {{name->printString(), boundSymbol->printString()}}));

                enumType = staticObjectCast<EnumType> (boundSymbol);
            }
        }

        // Check the symbol on the current lexical scope.
        if(!name)
        {
            auto previousLocalDefinition = environment->lexicalScope->lookupSymbolLocally(name);
            if(previousLocalDefinition)
                return recordSemanticErrorInNode(analyzedNode, formatString("Class ({1}) overrides previous definition in the same lexical scope ({2}).",
                    {{name->printString(), previousLocalDefinition->printString()}}));
        }
    }

    // Create the class type.
    if(!enumType)
    {
        enumType = basicMakeObject<EnumType> ();
        enumType->setName(name);
        enumType->setBaseType(AnyValue::__staticType__());
        enumType->setSupertypeAndImplicitMetaType(EnumTypeValue::__staticType__());
        enumType->setVisibility(node->visibility);
        enumType->setExternalLanguageMode(node->externalLanguageMode);
        enumType->setDllLinkageMode(node->dllLinkageMode);
        ownerEntity->recordChildProgramEntityDefinition(enumType);
        if(name)
            ownerEntity->bindProgramEntityWithVisibility(enumType, analyzedNode->visibility);
    }

    // Defer the value type analysis.
    if(analyzedNode->valueType)
    {
        enumType->enqueuePendingValueTypeCodeFragment(DeferredCompileTimeCodeFragment::make(analyzedNode->valueType, environment));
        analyzedNode->valueType.reset();
    }

    // Defer the values.
    if(analyzedNode->values)
    {
        enumType->enqueuePendingValuesCodeFragment(DeferredCompileTimeCodeFragment::make(analyzedNode->values, environment->copyForPublicProgramEntityBody(enumType)));
        analyzedNode->values.reset();
    }

    // Defer the body analysis.
    if(analyzedNode->body)
    {
        enumType->setSourceDefinitionPosition(node->sourcePosition);
        enumType->enqueuePendingBodyBlockCodeFragment(DeferredCompileTimeCodeFragment::make(analyzedNode->body, environment->copyForPublicProgramEntityBody(enumType)));
        analyzedNode->body.reset();
    }

    analyzedNode->analyzedProgramEntity = enumType;
    analyzedNode->analyzedType = enumType->getType();
    return analyzedNode;
}

void ASTSemanticAnalyzer::analyzeAndEvaluateAsValuesForEnumType(const ASTNodePtr &node, const EnumTypePtr &enumType)
{
    if(node->isASTMakeDictionaryNode())
    {
        auto makeDictionaryNode = staticObjectCast<ASTMakeDictionaryNode> (node);
        AnyValuePtr lastValue = nullptr;
        for(auto &element : makeDictionaryNode->elements)
        {
            sysmelAssert(element->isASTMakeAssociationNode());
            lastValue = analyzeAndEvaluateValueForEnumType(lastValue, staticObjectCast<ASTMakeAssociationNode> (element), enumType);
        }

        return;
    }

    auto analyzedNode = analyzeNodeIfNeededWithAutoType(node);
    enumType->addValues(analyzedNode, evaluateInCompileTime(analyzedNode));
}

AnyValuePtr ASTSemanticAnalyzer::analyzeAndEvaluateValueForEnumType(const AnyValuePtr &lastValue, const ASTMakeAssociationNodePtr &node, const EnumTypePtr &enumType)
{
    auto analyzedKey = analyzeNodeIfNeededWithAutoType(node->key);
    auto key = evaluateInCompileTime(analyzedKey);

    auto value = lastValue;
    auto valueNode = node->value;
    auto baseType = enumType->getBaseType();
    if(!valueNode)
    {
        if(baseType->isSubtypeOf(LiteralNumber::__staticType__()) ||
            baseType->isSubtypeOf(PrimitiveNumberType::__staticType__()))
        {
            auto builder = basicMakeObject<ASTBuilder> ();
            builder->sourcePosition = node->sourcePosition;
            valueNode = builder->sendToWithArguments(builder->literalSymbol("+"), builder->literal(lastValue), {
                builder->literal(LiteralInteger::makeFor(LargeInteger{1}))
            });
        }
    }

    if(valueNode)
    {
        auto enumScope = basicMakeObject<EnumRawValueLookupScope> ();
        enumScope->parent = environment->lexicalScope;
        enumScope->enumType = enumType;

        auto valueEnvironment = environment->copyWithLexicalScope(LexicalScope::makeWithParent(enumScope));
        auto analyzedValueNode = withEnvironmentDoAnalysis(valueEnvironment, [&](){
            return analyzeNodeIfNeededWithExpectedType(valueNode, baseType);
        });
        
        if(!analyzedValueNode->isASTErrorNode())
            value = evaluateInCompileTime(analyzedValueNode);
    }

    enumType->addValue(node, key, value);
    return value;
}

AnyValuePtr ASTSemanticAnalyzer::visitClassNode(const ASTClassNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

    auto name = evaluateNameSymbolValue(analyzedNode->name);
    auto ownerEntity = environment->programEntityForPublicDefinitions;

    ClassTypePtr classType;
    if(name)
    {
        if(isNameReserved(name))
            return recordSemanticErrorInNode(analyzedNode, formatString("Class {1} overrides reserved name.",
                    {{name->printString()}}));

        // Find an existing class with the same name.
        {
            auto boundSymbol = ownerEntity->lookupLocalSymbolFromScope(name, environment->lexicalScope);
            if(boundSymbol)
            {
                if(!boundSymbol->isClassType())
                    return recordSemanticErrorInNode(analyzedNode, formatString("Class ({1}) overrides previous non-class definition in its parent program entity ({2}).",
                        {{name->printString(), boundSymbol->printString()}}));

                classType = staticObjectCast<ClassType> (boundSymbol);
            }
        }

        // Check the symbol on the current lexical scope.
        if(!name)
        {
            auto previousLocalDefinition = environment->lexicalScope->lookupSymbolLocally(name);
            if(previousLocalDefinition)
                return recordSemanticErrorInNode(analyzedNode, formatString("Class ({1}) overrides previous definition in the same lexical scope ({2}).",
                    {{name->printString(), previousLocalDefinition->printString()}}));
        }
    }

    // Create the class type.
    if(!classType)
    {
        classType = basicMakeObject<ClassType> ();
        classType->setName(name);
        classType->setSupertypeAndImplicitMetaType(ClassTypeValue::__staticType__());
        classType->setVisibility(node->visibility);
        classType->setExternalLanguageMode(node->externalLanguageMode);
        classType->setDllLinkageMode(node->dllLinkageMode);
        ownerEntity->recordChildProgramEntityDefinition(classType);
        if(name)
            ownerEntity->bindProgramEntityWithVisibility(classType, analyzedNode->visibility);
    }

    // Defer the superclass analysis.
    if(analyzedNode->superclass)
    {
        classType->enqueuePendingSuperclassCodeFragment(DeferredCompileTimeCodeFragment::make(analyzedNode->superclass, environment));
        analyzedNode->superclass.reset();
    }

    // Defer the body analysis.
    if(analyzedNode->body)
    {
        classType->setSourceDefinitionPosition(node->sourcePosition);
        classType->enqueuePendingBodyBlockCodeFragment(DeferredCompileTimeCodeFragment::make(analyzedNode->body, environment->copyForPublicProgramEntityBody(classType)));
        analyzedNode->body.reset();
    }

    analyzedNode->analyzedProgramEntity = classType;
    analyzedNode->analyzedType = classType->getType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitStructNode(const ASTStructNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

    auto name = evaluateNameSymbolValue(analyzedNode->name);
    auto ownerEntity = environment->programEntityForPublicDefinitions;

    StructureTypePtr structureType;
    if(name)
    {
        if(isNameReserved(name))
            return recordSemanticErrorInNode(analyzedNode, formatString("Structure {1} overrides reserved name.",
                    {{name->printString()}}));

        // Find an existing structure with the same name.
        {
            auto boundSymbol = ownerEntity->lookupLocalSymbolFromScope(name, environment->lexicalScope);
            if(boundSymbol)
            {
                if(!boundSymbol->isStructureType())
                    return recordSemanticErrorInNode(analyzedNode, formatString("Structure ({1}) overrides previous non-structure definition in its parent program entity ({2}).",
                        {{name->printString(), boundSymbol->printString()}}));

                structureType = staticObjectCast<StructureType> (boundSymbol);
            }
        }

        // Check the symbol on the current lexical scope.
        if(!name)
        {
            auto previousLocalDefinition = environment->lexicalScope->lookupSymbolLocally(name);
            if(previousLocalDefinition)
                return recordSemanticErrorInNode(analyzedNode, formatString("Structure ({1}) overrides previous definition in the same lexical scope ({2}).",
                    {{name->printString(), previousLocalDefinition->printString()}}));
        }
    }

    // Create the structure type.
    if(!structureType)
    {
        structureType = basicMakeObject<StructureType> ();
        structureType->setName(name);
        structureType->setSupertypeAndImplicitMetaType(StructureTypeValue::__staticType__());
        structureType->setVisibility(node->visibility);
        structureType->setExternalLanguageMode(node->externalLanguageMode);
        structureType->setDllLinkageMode(node->dllLinkageMode);
        structureType->setPacked(node->isPacked);
        ownerEntity->recordChildProgramEntityDefinition(structureType);
        if(name)
            ownerEntity->bindProgramEntityWithVisibility(structureType, analyzedNode->visibility);
    }

    // Defer the body analysis.
    if(analyzedNode->body)
    {
        structureType->setSourceDefinitionPosition(node->sourcePosition);
        structureType->enqueuePendingBodyBlockCodeFragment(DeferredCompileTimeCodeFragment::make(analyzedNode->body, environment->copyForPublicProgramEntityBody(structureType)));
        analyzedNode->body.reset();
    }

    analyzedNode->analyzedProgramEntity = structureType;
    analyzedNode->analyzedType = structureType->getType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitUnionNode(const ASTUnionNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

    auto name = evaluateNameSymbolValue(analyzedNode->name);
    auto ownerEntity = environment->programEntityForPublicDefinitions;

    UnionTypePtr unionType;
    if(name)
    {
        if(isNameReserved(name))
            return recordSemanticErrorInNode(analyzedNode, formatString("Union {1} overrides reserved name.",
                    {{name->printString()}}));

        // Find an existing union with the same name.
        {
            auto boundSymbol = ownerEntity->lookupLocalSymbolFromScope(name, environment->lexicalScope);
            if(boundSymbol)
            {
                if(!boundSymbol->isUnionType())
                    return recordSemanticErrorInNode(analyzedNode, formatString("Union ({1}) overrides previous non-union definition in its parent program entity ({2}).",
                        {{name->printString(), boundSymbol->printString()}}));

                unionType = staticObjectCast<UnionType> (boundSymbol);
            }
        }

        // Check the symbol on the current lexical scope.
        if(!name)
        {
            auto previousLocalDefinition = environment->lexicalScope->lookupSymbolLocally(name);
            if(previousLocalDefinition)
                return recordSemanticErrorInNode(analyzedNode, formatString("Union ({1}) overrides previous definition in the same lexical scope ({2}).",
                    {{name->printString(), previousLocalDefinition->printString()}}));
        }
    }

    // Create the union type.
    if(!unionType)
    {
        unionType = basicMakeObject<UnionType> ();
        unionType->setName(name);
        unionType->setSupertypeAndImplicitMetaType(UnionTypeValue::__staticType__());
        unionType->setVisibility(node->visibility);
        unionType->setExternalLanguageMode(node->externalLanguageMode);
        unionType->setDllLinkageMode(node->dllLinkageMode);
        ownerEntity->recordChildProgramEntityDefinition(unionType);
        if(name)
            ownerEntity->bindProgramEntityWithVisibility(unionType, analyzedNode->visibility);
    }

    // Defer the body analysis.
    if(analyzedNode->body)
    {
        unionType->setSourceDefinitionPosition(node->sourcePosition);
        unionType->enqueuePendingBodyBlockCodeFragment(DeferredCompileTimeCodeFragment::make(analyzedNode->body, environment->copyForPublicProgramEntityBody(unionType)));
        analyzedNode->body.reset();
    }

    analyzedNode->analyzedProgramEntity = unionType;
    analyzedNode->analyzedType = unionType->getType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitProgramEntityExtensionNode(const ASTProgramEntityExtensionNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->programEntity = analyzeNodeIfNeededWithExpectedType(analyzedNode->programEntity, ProgramEntity::__staticType__());
    if(analyzedNode->programEntity->isASTErrorNode())
        return analyzedNode->programEntity;

    auto programEntity = evaluateInCompileTime(analyzedNode->programEntity);
    if(!programEntity || !programEntity->isProgramEntity())
        return recordSemanticErrorInNode(node, "Failed to evaluate in compile time the corresponding program entity.");

    analyzedNode->analyzedProgramEntity = staticObjectCast<ProgramEntity> (programEntity);
    analyzedNode->analyzedType = analyzedNode->analyzedProgramEntity->getType();

    if(analyzedNode->body)
    {
        analyzedNode->analyzedProgramEntity->enqueuePendingBodyBlockCodeFragment(DeferredCompileTimeCodeFragment::make(analyzedNode->body, environment->copyForPublicProgramEntityBody(analyzedNode->analyzedProgramEntity)));
        analyzedNode->body.reset();
    }

    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitExplicitCastNode(const ASTExplicitCastNodePtr &node)
{
    auto analyzedExpression = analyzeNodeIfNeededWithTemporaryAutoType(node->expression);
    auto targetTypeNode = evaluateTypeExpression(node->targetType);
    if(analyzedExpression->isASTErrorNode())
        return analyzedExpression;
    if(targetTypeNode->isASTErrorNode())
        return targetTypeNode;

    auto sourceType = analyzedExpression->analyzedType;
    auto targetType = staticObjectCast<Type> (
        targetTypeNode.staticAs<ASTLiteralValueNode> ()->value
    );

    auto typeConversionRule = sourceType->findExplicitTypeConversionRuleForInto(analyzedExpression, targetType);
    if(!typeConversionRule)
        return recordSemanticErrorInNode(analyzedExpression, formatString("Cannot perform implicit cast from '{0}' onto '{1}'.", {sourceType->printString(), targetType->printString()}));
    
    return typeConversionRule->convertNodeAtIntoWith(analyzedExpression, node->sourcePosition, targetType, selfFromThis());
}

AnyValuePtr ASTSemanticAnalyzer::visitImplicitCastNode(const ASTImplicitCastNodePtr &node)
{
    auto analyzedExpression = analyzeNodeIfNeededWithTemporaryAutoType(node->expression);
    auto targetTypeNode = evaluateTypeExpression(node->targetType);
    if(analyzedExpression->isASTErrorNode())
        return analyzedExpression;
    if(targetTypeNode->isASTErrorNode())
        return targetTypeNode;

    auto sourceType = analyzedExpression->analyzedType;
    auto targetType = staticObjectCast<Type> (
        targetTypeNode.staticAs<ASTLiteralValueNode> ()->value
    );

    auto typeConversionRule = sourceType->findImplicitTypeConversionRuleForInto(analyzedExpression, targetType);
    if(!typeConversionRule)
        return recordSemanticErrorInNode(analyzedExpression, formatString("Cannot perform implicit cast from '{0}' onto '{1}'.", {sourceType->printString(), targetType->printString()}));
    
    return typeConversionRule->convertNodeAtIntoWith(analyzedExpression, node->sourcePosition, targetType, selfFromThis());
}

AnyValuePtr ASTSemanticAnalyzer::visitReinterpretCastNode(const ASTReinterpretCastNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->expression = analyzeNodeIfNeededWithTemporaryAutoType(node->expression);
    analyzedNode->targetType = evaluateTypeExpression(node->targetType);
    if(analyzedNode->isASTErrorNode())
        return analyzedNode->expression;
    if(analyzedNode->isASTErrorNode())
        return analyzedNode->targetType;

    auto sourceType = analyzedNode->expression->analyzedType;
    auto targetType = unwrapTypeFromLiteralValue(analyzedNode->targetType);

    if(!sourceType->canBeReinterpretedAsType(targetType))
        return recordSemanticErrorInNode(analyzedNode, formatString("Cannot perform reinterpret cast from '{0}' onto '{1}'.", {sourceType->printString(), targetType->printString()}));

    analyzedNode->analyzedType = targetType;
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitTypeConversionNode(const ASTTypeConversionNodePtr &node)
{
    sysmelAssert(node->analyzedType);
    return node;
}

CompilationErrorPtr ASTSemanticAnalyzer::makeCompilationError()
{
    if(recordedErrors.empty())
        return nullptr;

    if(recordedErrors.size() == 1)
        return recordedErrors.back()->asCompilationError();

    auto errors = basicMakeObject<CompilationErrors> ();
    for(const auto &node : recordedErrors)
        errors->errors.push_back(node->asCompilationError());

    return errors;
}

AnyValuePtr ASTSemanticAnalyzer::visitIfNode(const ASTIfNodePtr &node)
{
    // If there are no branches, simplify by casting the condition to void.
    if(!node->trueExpression && !node->falseExpression)
        return analyzeNodeIfNeededWithExpectedType(node->condition, Type::getVoidType());

    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->condition = analyzeNodeIfNeededWithBooleanExpectedType(analyzedNode->condition);

    if(analyzedNode->trueExpression)
        analyzedNode->trueExpression = analyzeNodeIfNeededWithAutoType(analyzedNode->trueExpression);

    if(analyzedNode->falseExpression)
        analyzedNode->falseExpression = analyzeNodeIfNeededWithAutoType(analyzedNode->falseExpression);

    analyzedNode->analyzedType = Type::getVoidType();

    if(analyzedNode->trueExpression && analyzedNode->falseExpression)
    {
        // TODO: Compute a proper coercion type.
        if(analyzedNode->trueExpression->analyzedType == analyzedNode->falseExpression->analyzedType)
            analyzedNode->analyzedType = analyzedNode->trueExpression->analyzedType;
    }
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitWhileNode(const ASTWhileNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    if(analyzedNode->condition)
        analyzedNode->condition = analyzeNodeIfNeededWithBooleanExpectedType(analyzedNode->condition);

    if(analyzedNode->bodyExpression)
    {
        // Increment the continue and break level count.
        auto bodyEnvironment = environment->copyWithBreakAndContinueLevel(environment->breakLevelCount + 1, environment->continueLevelCount + 1);
        analyzedNode->bodyExpression = withEnvironmentDoAnalysis(bodyEnvironment, [&]() {
            return analyzeNodeIfNeededWithExpectedType(analyzedNode->bodyExpression, Type::getVoidType());
        });
    }

    if(analyzedNode->continueExpression)
        analyzedNode->continueExpression = analyzeNodeIfNeededWithExpectedType(analyzedNode->continueExpression, Type::getVoidType());

    analyzedNode->analyzedType = Type::getVoidType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitDoWhileNode(const ASTDoWhileNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);

    if(analyzedNode->bodyExpression)
    {
        // Increment the continue and break level count.
        auto bodyEnvironment = environment->copyWithBreakAndContinueLevel(environment->breakLevelCount + 1, environment->continueLevelCount + 1);
        analyzedNode->bodyExpression = withEnvironmentDoAnalysis(bodyEnvironment, [&]() {
            return analyzeNodeIfNeededWithExpectedType(analyzedNode->bodyExpression, Type::getVoidType());
        });
    }

    if(analyzedNode->condition)
        analyzedNode->condition = analyzeNodeIfNeededWithBooleanExpectedType(analyzedNode->condition);

    if(analyzedNode->continueExpression)
        analyzedNode->continueExpression = analyzeNodeIfNeededWithExpectedType(analyzedNode->continueExpression, Type::getVoidType());

    analyzedNode->analyzedType = Type::getVoidType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitReturnNode(const ASTReturnNodePtr &node)
{
    auto analyzedNode = shallowCloneObject(node);
    auto returnTargetMethod = environment->returnTargetMethod;
    if(!returnTargetMethod)
    {
        // Discover further errors.
        if(analyzedNode->expression)
            analyzeNodeIfNeededWithAutoType(analyzedNode->expression);

        return recordSemanticErrorInNode(analyzedNode, "Cannot use a return statement in this location.");
    }

    auto expectedResultType = returnTargetMethod->getFunctionalType()->getResultType();
    if(analyzedNode->expression)
    {
        if(expectedResultType->isAutoType())
        {
            analyzedNode->expression = analyzeNodeIfNeededWithAutoType(analyzedNode->expression);
            expectedResultType = returnTargetMethod->getFunctionalType()->getResultType();
        }

        if(expectedResultType->isAutoType())
        {
            auto inferredType = analyzedNode->expression->analyzedType;
            if(inferredType->isControlFlowEscapeType())
            {
                analyzedNode->analyzedType = inferredType;
                return analyzedNode;
            }

            returnTargetMethod->concretizeAutoResultTypeWith(inferredType);
        }
        else
        {
            analyzedNode->expression = analyzeNodeIfNeededWithExpectedType(analyzedNode->expression, expectedResultType);
        }
    }
    else
    {
        if(expectedResultType->isAutoType())
            returnTargetMethod->concretizeAutoResultTypeWith(Type::getVoidType());
        else if(!expectedResultType->isVoidType())
            return recordSemanticErrorInNode(analyzedNode, "A non-void result expression is required.");
    }

    analyzedNode->analyzedType = Type::getReturnType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitContinueNode(const ASTContinueNodePtr &node)
{
    if(environment->continueLevelCount == 0)
        return recordSemanticErrorInNode(node, "Cannot use a continue statement in this location.");

    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->analyzedType = Type::getContinueType();
    return analyzedNode;
}

AnyValuePtr ASTSemanticAnalyzer::visitBreakNode(const ASTBreakNodePtr &node)
{
    if(environment->breakLevelCount == 0)
        return recordSemanticErrorInNode(node, "Cannot use a break statement in this location.");

    auto analyzedNode = shallowCloneObject(node);
    analyzedNode->analyzedType = Type::getBreakType();
    return analyzedNode;
}


} // End of namespace Environment
} // End of namespace Sysmel