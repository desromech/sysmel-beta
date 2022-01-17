#include "Environment/CompiledMethod.hpp"
#include "Environment/ASTNode.hpp"
#include "Environment/CannotEvaluateMessageInCompileTime.hpp"
#include "Environment/CannotEvaluateUndefinedMessage.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/CompilationError.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTCompileTimeEvaluator.hpp"
#include "Environment/ASTSSACompiler.hpp"

#include "Environment/SSAFunction.hpp"
#include "Environment/SSACodeRegion.hpp"
#include "Environment/SSACodeRegionArgument.hpp"
#include "Environment/SSACodeRegionCapture.hpp"

#include "Environment/ASTAnalysisEnvironment.hpp"
#include "Environment/CleanUpScope.hpp"
#include "Environment/LexicalScope.hpp"
#include "Environment/ProgramEntityScope.hpp"
#include "Environment/ReceiverMemberLookupScope.hpp"
#include "Environment/CompileTimeCleanUpScope.hpp"

#include "Environment/FunctionalType.hpp"
#include "Environment/ArgumentVariable.hpp"
#include "Environment/ArgumentCountError.hpp"
#include "Environment/Assert.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<CompiledMethod> compiledMethodTypeRegistration;

bool CompiledMethod::isCompiledMethod() const
{
    return true;
}

bool CompiledMethod::isDefined() const
{
    return definitionEnvironment || analyzedBodyNode;
}

bool CompiledMethod::isDefinedForCompileTime() const
{
    return isDefined();
}

void CompiledMethod::setDeclaration(const ASTNodePtr &node)
{
    declarationNode = node;
    declarationPosition = node->sourcePosition;
}

void CompiledMethod::setDefinition(const ASTNodePtr &node, const ASTNodePtr &bodyNode, const ASTAnalysisEnvironmentPtr &environment)
{
    if(!declarationNode)
        setDeclaration(node);

    definitionNode = node;
    definitionEnvironment = environment;
    definitionPosition = node->sourcePosition;
    definitionBodyNode = bodyNode;
}

ASTAnalysisEnvironmentPtr CompiledMethod::createSemanticAnalysisEnvironment()
{
    auto result = definitionEnvironment->copyWithCleanUpcope(CleanUpScope::makeEmpty());
    IdentifierLookupScopePtr parentScope = ProgramEntityScope::make(definitionEnvironment->lexicalScope, selfFromThis());

    if(receiverArgument)
    {
        auto receiverScope = basicMakeObject<ReceiverMemberLookupScope> ();
        receiverScope->parent = parentScope;
        receiverScope->receiverType = receiverArgument->getValueType();
        receiverScope->receiverVariable = receiverArgument;
        parentScope = receiverScope;
    }

    auto lexicalScope = LexicalScope::makeWithParent(parentScope, definitionPosition);
    result->lexicalScope = lexicalScope;
    result->returnTargetMethod = selfFromThis();
    result->localDefinitionsOwner = selfFromThis();

    // Add the receiver argument
    if(receiverArgument)
    {
        if(isMacroMethod())
        {
            if(validAnyValue(receiverArgument->getName())->isAnonymousNameSymbol())
                receiverArgument->setName(internSymbol("__macroContext"));
        }
        else
        {
            if(validAnyValue(receiverArgument->getName())->isAnonymousNameSymbol())
                receiverArgument->setName(internSymbol("self"));
        }

        lexicalScope->setSymbolBinding(receiverArgument->getName(), receiverArgument);
    }

    // Add the arguments to the lexical scope.
    for(auto &arg : arguments)
    {
        if(arg->getName())
            lexicalScope->setSymbolBinding(arg->getName(), arg);
    }

    return result;
}

void CompiledMethod::setMethodSignature(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &argumentTypes)
{
    SuperType::setMethodSignature(receiverType, resultType, argumentTypes);
    createImplicitReceiverArgumentVariableWithType(receiverType);
    createArgumentVariablesWithTypes(argumentTypes);
}

void CompiledMethod::setFunctionSignature(const TypePtr &resultType, const TypePtrList &argumentTypes)
{
    SuperType::setFunctionSignature(resultType, argumentTypes);
    createArgumentVariablesWithTypes(argumentTypes);
}

void CompiledMethod::setClosureSignature(const TypePtr &resultType, const TypePtrList &argumentTypes)
{
    SuperType::setClosureSignature(resultType, argumentTypes);
    createArgumentVariablesWithTypes(argumentTypes);
}

void CompiledMethod::createImplicitReceiverArgumentVariableWithType(const TypePtr &receiverType)
{
    if(receiverType->isVoidType())
        return;

    receiverArgument = basicMakeObject<ArgumentVariable> ();
    receiverArgument->isImplicit = true;
    receiverArgument->isReceiver = true;
    recordChildProgramEntityDefinition(receiverArgument);
    receiverArgument->setType(receiverType);
}

void CompiledMethod::createArgumentVariablesWithTypes(const TypePtrList &argumentTypes)
{
    arguments.reserve(argumentTypes.size());
    size_t firstArgumentIndex = receiverArgument ? 1 : 0;
    for(size_t i = 0; i < argumentTypes.size(); ++i)
    {
        const auto &type = argumentTypes[i];
        auto argument = basicMakeObject<ArgumentVariable> ();
        recordChildProgramEntityDefinition(argument);
        argument->setType(type);
        argument->argumentIndex = uint32_t(i + firstArgumentIndex);
        arguments.push_back(argument);
    }
}

void CompiledMethod::setArgumentDeclarationNode(size_t index, const ASTArgumentDefinitionNodePtr &argumentNode)
{
    sysmelAssert(index < arguments.size());
    arguments[index]->setArgumentDeclarationNode(argumentNode);
}

void CompiledMethod::setArgumentDefinitionNode(size_t index, const ASTArgumentDefinitionNodePtr &argumentNode)
{
    sysmelAssert(index < arguments.size());
    arguments[index]->setArgumentDefinitionNode(argumentNode);
}

ASTNodePtr CompiledMethod::analyzeDefinitionWith(const ASTSemanticAnalyzerPtr &analyzer)
{
    if(analyzedBodyNode)
        return analyzedBodyNode;

    analyzedBodyEnvironment = createSemanticAnalysisEnvironment();
    analyzedBodyNode = analyzer->withEnvironmentDoAnalysis(analyzedBodyEnvironment, [&]() {
        return analyzer->analyzeNodeIfNeededWithExpectedType(definitionBodyNode, functionalType->getResultType());
    });

    concretizeAutoResultTypeWith(analyzedBodyNode->analyzedType);
    return analyzedBodyNode;
}

void CompiledMethod::ensureSemanticAnalysis()
{
    if(analyzedBodyNode || !isDefined())
        return;

    {
        auto analyzer = basicMakeObject<ASTSemanticAnalyzer> ();
        analyzeDefinitionWith(analyzer);
        auto compilationError = analyzer->makeCompilationError();
        if(compilationError)
            compilationError->signal();
    }

    asSSAValueRequiredInPosition(nullptr);
}

const ArgumentVariablePtr &CompiledMethod::getReceiverArgument() const
{
    return receiverArgument;
}

const ArgumentVariablePtrList &CompiledMethod::getArguments() const
{
    return arguments;
}

void CompiledMethod::recordCapturedFunctionVariable(const FunctionVariablePtr &capturedVariable)
{
    if(capturedVariableSet.find(capturedVariable) != capturedVariableSet.end())
        return;

    // Make sure that my parent also captures the variable.
    auto parentEntity = getParentProgramEntity();
    auto variableParentEntity = capturedVariable->getParentProgramEntity();
    sysmelAssert(variableParentEntity->isCompiledMethod());
    if(parentEntity->isCompiledMethod() && parentEntity != variableParentEntity)
        parentEntity.staticAs<CompiledMethod> ()->recordCapturedFunctionVariable(capturedVariable);

    // Record the captured variable.
    capturedVariables.push_back(capturedVariable);
    capturedVariableSet.insert(capturedVariable);
}

const FunctionVariablePtrList &CompiledMethod::getCapturedVariables() const
{
    return capturedVariables;
}

SSAValuePtr CompiledMethod::asSSAValueRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition)
{
    (void)requiredSourcePosition;
    if(ssaCompiledFunction)
        return ssaCompiledFunction;

    // Make the function.    
    ssaCompiledFunction = basicMakeObject<SSAFunction> ();
    ssaCompiledFunction->setExternalLanguageMode(externalLanguageMode);
    ssaCompiledFunction->setVisibility(visibility);
    ssaCompiledFunction->setDllLinkageMode(dllLinkageMode);
    ssaCompiledFunction->initializeWithNameAndType(getName(), functionalType);
    ssaCompiledFunction->setIntrinsicName(getIntrinsicName());
    ssaCompiledFunction->setDeclarationPosition(declarationPosition);
    ssaCompiledFunction->setSourceProgramEntity(selfFromThis());
    ssaCompiledFunction->setCompileTime(isCompileTimeMethod());

    ensureSemanticAnalysis();

    // Add the function onto its ssa parent.
    auto parentEntity = getParentProgramEntity();
    // FIXME: Add the top-level script methods some-where.
    if(parentEntity)
    {
        auto parentSSAValue = parentEntity->asProgramEntitySSAValue();
        sysmelAssert(parentSSAValue->isSSAProgramEntity());
        parentSSAValue.staticAs<SSAProgramEntity> ()->addChild(ssaCompiledFunction);
    }

    auto mainCodeRegion = ssaCompiledFunction->getMainCodeRegion();

    // Set the argument metadata.
    size_t receiverOffset = functionalType->getResultType()->isReturnedByReference() ? 1 : 0;
    size_t argumentsOffset = receiverOffset;
    auto receiverType = functionalType->getReceiverType();
    if(!receiverType->isVoidType())
    {
        auto ssaArg = mainCodeRegion->getArgument(receiverOffset);
        ssaArg->setDeclarationPosition(declarationPosition);
        ssaArg->setVariable(receiverArgument);
        ++argumentsOffset;
    }

    for(size_t i = 0; i < arguments.size(); ++i)
    {
        auto ssaArg = mainCodeRegion->getArgument(i + argumentsOffset);
        ssaArg->setDeclarationPosition(arguments[i]->getDeclarationPosition());
        ssaArg->setVariable(arguments[i]);
    }

    // Add the captured variables.
    for(auto &capturedVariable : capturedVariables)
    {
        auto capture = mainCodeRegion->addCaptureWithType(capturedVariable->getReferenceType());
        capture->setDeclarationPosition(capturedVariable->getDeclarationPosition());
        capture->setDefinitionPosition(capturedVariable->getDefinitionPosition());
    }

    if(analyzedBodyNode)
    {
        ssaCompiledFunction->setDefinitionPosition(definitionPosition);
        if(!receiverType->isVoidType())
            mainCodeRegion->getArgument(receiverOffset)->setDefinitionPosition(declarationPosition);
        for(size_t i = 0; i < arguments.size(); ++i)
            mainCodeRegion->getArgument(i + argumentsOffset)->setDefinitionPosition(arguments[i]->getDefinitionPosition());

        auto compiler = basicMakeObject<ASTSSACompiler> ();
        compiler->compileMethodBody(selfFromThis(), ssaCompiledFunction, analyzedBodyNode, analyzedBodyEnvironment);
    }

    return ssaCompiledFunction;
}

std::string CompiledMethod::fullPrintString() const
{
    return sexpressionToPrettyString(asFullDefinitionSExpression());
}

SExpression CompiledMethod::asSExpression() const
{
    return SExpressionList{{
        SExpressionIdentifier{"compiledMethod"},
        validAnyValue(name)->asSExpression(),
        functionalType->asSExpression()
    }};
}

SExpression CompiledMethod::asFullDefinitionSExpression() const
{
    SExpressionList childrenSExpr;
    childrenSExpr.elements.reserve(children.size());
    for(auto &child : children)
        childrenSExpr.elements.push_back(child->asFullDefinitionSExpression());

    SExpression bodySExpr = SExpressionVoid{};
    if(analyzedBodyNode)
        bodySExpr = analyzedBodyNode->asSExpression();

    return SExpressionList{{
        SExpressionIdentifier{"compiledMethod"},
        validAnyValue(name)->asSExpression(),
        functionalType->asSExpression(),
        childrenSExpr,
        bodySExpr
    }};
}

void CompiledMethod::validateBeforeCompileTimeEvaluation()
{
    if(!isDefined())
        signalNew<CannotEvaluateUndefinedMessage> ();
    if(!isDefinedForCompileTime())
        signalNew<CannotEvaluateMessageInCompileTime> ();
    ensureSemanticAnalysis();
}

AnyValuePtr CompiledMethod::runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &argumentValues, const AnyValuePtr &receiver)
{
    sysmelAssert(!functionalType->isClosureType());
    (void)selector;
    
    validateBeforeCompileTimeEvaluation();

    auto evaluationEnvironment = CompileTimeCleanUpScope::makeEmpty();
    setArgumentsInEvaluationEnvironment(receiver, argumentValues, evaluationEnvironment);
    return basicMakeObject<ASTCompileTimeEvaluator> ()->evaluateMethodBodyNode(evaluationEnvironment, analyzedBodyNode);
}

AnyValuePtr CompiledMethod::applyInClosureWithArguments(const AnyValuePtr &closure, const std::vector<AnyValuePtr> &argumentValues)
{
    sysmelAssert(functionalType->isClosureType());
    sysmelAssert(closure->isCompileTimeCleanUpScope());
    validateBeforeCompileTimeEvaluation();

    auto evaluationEnvironment = CompileTimeCleanUpScope::makeWithParent(staticObjectCast<CompileTimeCleanUpScope> (closure));
    setArgumentsInEvaluationEnvironment(nullptr, argumentValues, evaluationEnvironment);
    return basicMakeObject<ASTCompileTimeEvaluator> ()->evaluateMethodBodyNode(evaluationEnvironment, analyzedBodyNode);
}

void CompiledMethod::setArgumentsInEvaluationEnvironment(const AnyValuePtr &receiver, const AnyValuePtrList &argumentValues, const CompileTimeCleanUpScopePtr &environment)
{
    auto expectedReceiverType = functionalType->getReceiverType();
    if(!expectedReceiverType->isVoidType())
    {
        sysmelAssert(receiverArgument);
        environment->setStoreBinding(receiverArgument, receiver);
    }

    // Check the argument count.
    if(arguments.size() != argumentValues.size())
    {
        auto error = basicMakeObject<ArgumentCountError> ();
        error->expectedCount = functionalType->getArgumentCount();
        error->callCount = arguments.size();
        error->signal();
    }

    // Set the different arguments.
    for(size_t i = 0; i < arguments.size(); ++i)
        environment->setStoreBinding(arguments[i], argumentValues[i]);
}

AnyValuePtr CompiledMethod::applyWithArguments(const std::vector<AnyValuePtr> &argumentValues)
{
    sysmelAssert(!functionalType->isClosureType());
    validateBeforeCompileTimeEvaluation();
    auto evaluationEnvironment = CompileTimeCleanUpScope::makeEmpty();
    setArgumentsInEvaluationEnvironment(nullptr, argumentValues, evaluationEnvironment);
    return basicMakeObject<ASTCompileTimeEvaluator> ()->evaluateMethodBodyNode(evaluationEnvironment, analyzedBodyNode);
}

void CompiledMethod::recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild)
{
    children.push_back(newChild);
    newChild->setParentProgramEntity(selfFromThis());
}

} // End of namespace Environment
} // End of namespace Sysmel