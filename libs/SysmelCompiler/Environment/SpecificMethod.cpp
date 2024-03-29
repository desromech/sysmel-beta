#include "Environment/SpecificMethod.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTIdentifierReferenceNode.hpp"
#include "Environment/ASTLocalImmutableAccessNode.hpp"
#include "Environment/ASTErrorNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/Type.hpp"
#include "Environment/FunctionType.hpp"
#include "Environment/MethodType.hpp"
#include "Environment/ClosureType.hpp"
#include "Environment/PatternMatchingMethod.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/TemporaryReferenceType.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include <iostream>

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<SpecificMethod> specificMethodTypeRegistration;

bool SpecificMethod::isSpecificMethod() const
{
    return true;
}

bool SpecificMethod::isClosureMethod() const
{
    return functionalType && functionalType->isClosureType();
}

void SpecificMethod::setMethodSignature(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &argumentTypes)
{
    functionalType = getOrCreateMethodType(receiverType, resultType, argumentTypes);
    methodFlags = methodFlags | MethodFlags::MessageMethod;
}

void SpecificMethod::setFunctionSignature(const TypePtr &resultType, const TypePtrList &argumentTypes)
{
    functionalType = getOrCreateFunctionType(resultType, argumentTypes);
}

void SpecificMethod::setClosureSignature(const TypePtr &resultType, const TypePtrList &argumentTypes)
{
    functionalType = getOrCreateClosureType(resultType, argumentTypes);
}

MethodPatternMatchingResult SpecificMethod::matchPatternForRunWithIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
{
    (void)selector;
    // Make sure the argument count matches.
    if(arguments.size() != functionalType->getArgumentCount())
        return MethodPatternMatchingResult{};

    PatternMatchingRank matchingRank;

    // Receiver rank
    if(functionalType->getReceiverType()->isVoidType())
    {
        matchingRank.push_back(TypeConversionCost{receiver && !receiver->isVoid() ? DirectTypeConversionCost::ValueToVoid : DirectTypeConversionCost::Identity});
    }
    else
    {
        auto receiverRank = functionalType->getReceiverType()->rankToMatchValue(receiver);
        if(receiverRank.isInvalid())
            return MethodPatternMatchingResult{};
        matchingRank.push_back(receiverRank);
    }

    // Arguments rank.
    for(size_t i = 0; i < arguments.size(); ++i)
    {
        auto argumentRank = functionalType->getArgument(i)->rankToMatchValue(arguments[i]);
        if(argumentRank.isInvalid())
            return MethodPatternMatchingResult{};
        matchingRank.push_back(argumentRank);
    }

    return MethodPatternMatchingResult{selfFromThis(), matchingRank};
}

TypePtr SpecificMethod::getExpectedTypeForAnalyzingArgumentWithIndex(size_t argumentIndex)
{
    return argumentIndex < functionalType->getArgumentCount() ? functionalType->getArgument(argumentIndex) : nullptr;
}

MethodPatternMatchingResult SpecificMethod::matchPatternForAnalyzingMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    // Make sure the argument count matches.
    if(node->arguments.size() != functionalType->getArgumentCount())
        return MethodPatternMatchingResult{};

    PatternMatchingRank matchingRank;

    // Receiver rank
    if(functionalType->getReceiverType()->isVoidType())
    {
        // Ignore the receiver that we got.
        matchingRank.push_back(TypeConversionCost{node->receiver ? DirectTypeConversionCost::ValueToVoid : DirectTypeConversionCost::Identity});
    }
    else
    {
        // A receiver is required in this case.
        if(!node->receiver)
            return MethodPatternMatchingResult{};

        auto receiverRank = semanticAnalyzer->rankForMatchingTypeWithNode(functionalType->getReceiverType(), node->receiver, true);
        if(receiverRank.isInvalid())
            return MethodPatternMatchingResult{};
        matchingRank.push_back(receiverRank);
    }

    // Arguments rank
    for(size_t i = 0; i < node->arguments.size(); ++i)
    {
        auto expectedArgumentType = functionalType->getArgument(i);
        auto argumentRank = semanticAnalyzer->rankForMatchingTypeWithNode(expectedArgumentType, node->arguments[i]);
        if(argumentRank.isInvalid())
            return MethodPatternMatchingResult{};
        matchingRank.push_back(argumentRank);
    }

    return MethodPatternMatchingResult{selfFromThis(), matchingRank};
}

MethodPatternMatchingResult SpecificMethod::matchPatternForAnalyzingCallNode(const ASTCallNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    // Make sure the argument count matches.
    if(node->arguments.size() != functionalType->getArgumentCount())
        return MethodPatternMatchingResult{};

    PatternMatchingRank matchingRank;

    // Receiver rank
    if(!functionalType->getReceiverType()->isVoidType())
        return MethodPatternMatchingResult{};
    matchingRank.push_back(TypeConversionCost{DirectTypeConversionCost::Identity});

    // Arguments rank
    for(size_t i = 0; i < node->arguments.size(); ++i)
    {
        auto expectedArgumentType = functionalType->getArgument(i);
        auto argumentRank = semanticAnalyzer->rankForMatchingTypeWithNode(expectedArgumentType, node->arguments[i]);
        if(argumentRank.isInvalid())
            return MethodPatternMatchingResult{};
        matchingRank.push_back(argumentRank);
    }

    return MethodPatternMatchingResult{selfFromThis(), matchingRank};
}

AnyValuePtr SpecificMethod::asMethodMatchingDefinitionSignature(bool hasReceiver, bool hasConstReceiver, const TypePtrList &argumentTypes, const TypePtr &resultType)
{
    return functionalType && functionalType->matchesDefinitionSignature(hasReceiver, hasConstReceiver, argumentTypes, resultType) ? selfFromThis() : nullptr;  
}

AnyValuePtr SpecificMethod::asMethodMatchingSignature(const TypePtr &receiverType, const TypePtrList &argumentTypes, const TypePtr &resultType)
{
    return functionalType && functionalType->matchesSignature(receiverType, argumentTypes, resultType) ? selfFromThis() : nullptr;
}

bool SpecificMethod::isMacroMethod() const
{
    return functionalType->getReceiverType()
        && functionalType->getReceiverType()->isSubtypeOf(MacroInvocationContext::__staticType__())
        && ((methodFlags & MethodFlags::Macro) != MethodFlags::None);
}

ASTNodePtr SpecificMethod::analyzeMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(functionalType->getArgumentCount() != node->arguments.size())
        return semanticAnalyzer->recordSemanticErrorInNode(node, "Message argument count mismatch.");

    ASTNodePtr errorNode;
    node->selector = semanticAnalyzer->analyzeNodeIfNeededWithAutoType(node->selector);

    // Is this a macro method?
    if(isMacroMethod())
    {
        AnyValuePtr macroSelector;
        if(node->selector && node->selector->isASTLiteralSymbolValue())
            macroSelector = staticObjectCast<ASTLiteralValueNode> (node->selector)->value;

        auto macroInvocationContext = semanticAnalyzer->makeMacroInvocationContextFor(node);
        macroInvocationContext->selfType = adjustMacroInvocationSelfType(macroInvocationContext->selfType);

        AnyValuePtrList macroArguments;
        macroArguments.reserve(node->arguments.size());

        // Analyze each one of the arguments
        for(size_t i = 0; i < node->arguments.size(); ++i)
        {
            auto expectedType = functionalType->getArgument(i);
            auto argument = node->arguments[i];
            auto adaptedArgument = semanticAnalyzer->adaptNodeAsMacroArgumentOfType(argument, expectedType);
            if(adaptedArgument->isASTErrorNode())
                errorNode = staticObjectCast<ASTErrorNode> (adaptedArgument);

            macroArguments.push_back(adaptedArgument);
        }

        // Propagate error nodes.
        if(errorNode)
            return errorNode;

        // Evaluate the macro macro method.
        auto macroResultNode = semanticAnalyzer->guardCompileTimeEvaluationForNode(node, [&]() {
            auto macroDirectResult = runWithArgumentsIn(macroSelector, macroArguments, macroInvocationContext);
            return validAnyValue(macroDirectResult)->asASTNodeRequiredInPosition(node->sourcePosition);
        });

        // Analyze the macro expanded node.
        return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(macroResultNode);
    }

    // Analyze the receiver.
    if(node->receiver)
    {
        node->receiver = semanticAnalyzer->analyzeNodeIfNeededWithExpectedReceiverType(node->receiver, functionalType->getReceiverType());
        if(node->receiver->isASTErrorNode())
            errorNode = node->receiver;
    }

    // Analyze each one of the arguments
    for(size_t i = 0; i < node->arguments.size(); ++i)
    {
        auto expectedType = functionalType->getArgument(i);
        auto &argument = node->arguments[i];
        argument = semanticAnalyzer->analyzeNodeIfNeededWithExpectedType(argument, expectedType);
        if(argument->isASTErrorNode())
            errorNode = argument;
    }

    // Propagate error nodes.
    if(errorNode)
        return errorNode;

    auto receiverType = functionalType->getReceiverType();
    node->analyzedBoundMessageIsDirect = isConstructor() || isConversion() || !receiverType->supportsDynamicCompileTimeMessageSend() ;
    node->useVirtualTable = hasVirtualSendSemantics() && !isFinal();
    node->analyzedBoundMessageIsDirect = node->analyzedBoundMessageIsDirect && !node->useVirtualTable;

    // Be always direct when going through a super reference.
    if(node->receiver && node->receiver->isSuperReference())
    {
        node->useVirtualTable = false;
        node->analyzedBoundMessageIsDirect = true;
    }

    node->analyzedBoundMessage = selfFromThis();
    node->calledMessageType = getFunctionalType();
    node->analyzedType = functionalType->getResultType();
    node->isPureMessageSend = isPure();

    if(node->useVirtualTable)
        receiverType->asDecayedType()->ensureVirtualTableLayoutComputation();
        
    node->virtualTableSlotIndex = virtualTableSlotIndex;
    node->virtualTableEntrySlotIndex = virtualTableEntrySlotIndex;

    return semanticAnalyzer->optimizeAnalyzedMessageSend(node);
}

ASTNodePtr SpecificMethod::analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(isMacroMethod() && functionalType->getArgumentCount() == 0)
    {
        auto macroInvocationContext = semanticAnalyzer->makeMacroInvocationContextFor(node);
        macroInvocationContext->selfType = adjustMacroInvocationSelfType(macroInvocationContext->selfType);

        // Evaluate the macro macro method.
        auto macroResultNode = semanticAnalyzer->guardCompileTimeEvaluationForNode(node, [&]() {
            auto macroDirectResult = runWithArgumentsIn(node->identifier, {}, macroInvocationContext);
            return validAnyValue(macroDirectResult)->asASTNodeRequiredInPosition(node->sourcePosition);
        });

        // Analyze the macro expanded node.
        return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(macroResultNode);
    }

    // Is this a closure?
    if(functionalType->isClosureType())
    {
        auto result = basicMakeObject<ASTLocalImmutableAccessNode> ();
        result->sourcePosition = node->sourcePosition;
        result->bindingName = selfFromThis();
        result->analyzedType = functionalType;
        return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(result);
    }
    
    return asFunctionalValue()->analyzeIdentifierReferenceNode(node, semanticAnalyzer);
}

ASTNodePtr SpecificMethod::analyzeCallNode(const ASTCallNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    auto newCallNode = shallowCloneObject(node);
    auto newCallNodeFunctionalValue = asFunctionalValue();
    newCallNode->function = validAnyValue(newCallNodeFunctionalValue)->asASTNodeRequiredInPosition(node->sourcePosition);
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(newCallNode);
}

FunctionalTypeValuePtr SpecificMethod::asFunctionalValue()
{
    if(functionalType->isClosureType())
        return nullptr;

    if(!functionalValue)
        functionalValue = functionalType->makeValueWithImplementation(selfFromThis());
    return functionalValue;
}

SExpression SpecificMethod::asSExpression() const
{
    return SExpressionList{{SExpressionIdentifier{{"specificMethod"}},
        validAnyValue(name)->asSExpression(),
        validAnyValue(intrinsicName)->asSExpression(),
    }};
}

void SpecificMethod::setIntrinsicName(const AnyValuePtr &theIntrinsicName)
{
    intrinsicName = theIntrinsicName;
}

const AnyValuePtr &SpecificMethod::getIntrinsicName() const
{
    return intrinsicName;
}

void SpecificMethod::concretizeAutoResultTypeWith(const TypePtr &newResultType)
{
    if(!functionalType->hasAutoResultType())
        return;
    
    sysmelAssert(!newResultType->isReturnType());
    functionalType = functionalType->copyWithResultType(newResultType);
    if(functionalValue)
        functionalValue->type = functionalType;
}

void SpecificMethod::addMethodFlags(MethodFlags extraMethodFlags)
{
    methodFlags = methodFlags | extraMethodFlags;
}

MethodFlags SpecificMethod::getMethodFlags() const
{
    return methodFlags;
}

void SpecificMethod::setMethodFlags(MethodFlags newFlags)
{
    methodFlags = newFlags;
}

bool SpecificMethod::isConstructor() const
{
    return (methodFlags & MethodFlags::Constructor) != MethodFlags::None;
}

void SpecificMethod::makeConstructor()
{
    methodFlags = methodFlags | MethodFlags::Constructor;
}

bool SpecificMethod::isExplicit() const
{
    return (methodFlags & MethodFlags::Explicit) != MethodFlags::None;
}

void SpecificMethod::makeExplicit()
{
    methodFlags = methodFlags | MethodFlags::Explicit;
}

bool SpecificMethod::isPure() const
{
    return (methodFlags & MethodFlags::Pure) != MethodFlags::None;
}

void SpecificMethod::makePure()
{
    methodFlags = methodFlags | MethodFlags::Pure;
}

bool SpecificMethod::isConversion() const
{
    return (methodFlags & MethodFlags::Conversion) != MethodFlags::None;
}

void SpecificMethod::makeConversion()
{
    methodFlags = methodFlags | MethodFlags::Conversion;
}

bool SpecificMethod::isTrivial() const
{
    return (methodFlags & MethodFlags::Trivial) != MethodFlags::None;
}

void SpecificMethod::makeTrivial()
{
    methodFlags = methodFlags | MethodFlags::Trivial;
}

bool SpecificMethod::isCompileTimeMethod() const
{
    if((methodFlags & MethodFlags::NotInCompileTime) != MethodFlags::None)
        return false;

    return (methodFlags & MethodFlags::CompileTime) != MethodFlags::None || isMacroMethod();
}

bool SpecificMethod::hasVirtualSendSemantics() const
{
    return (methodFlags & MethodFlags::VirtualSendFlags) != MethodFlags::None;
}

bool SpecificMethod::isAbstract() const
{
    return (methodFlags & MethodFlags::VirtualSendFlags) != MethodFlags::None;
}

bool SpecificMethod::isVirtual() const
{
    return (methodFlags & MethodFlags::Virtual) != MethodFlags::None;
}

bool SpecificMethod::isOverride() const
{
    return (methodFlags & MethodFlags::Override) != MethodFlags::None;
}

bool SpecificMethod::isFinal() const
{
    return (methodFlags & MethodFlags::Final) != MethodFlags::None;
}

bool SpecificMethod::isStatic() const
{
    return (methodFlags & MethodFlags::Static) != MethodFlags::None;
}

const SpecificMethodPtr &SpecificMethod::getOverridenParentMethod()
{
    return overridenParentMethod;
}

void SpecificMethod::setOverridenParentMethod(const SpecificMethodPtr &newParentMethod)
{
    overridenParentMethod = newParentMethod;
}

void SpecificMethod::setVirtualTableEntry(uint32_t newVirtualTableSlotIndex, uint32_t newVirtualTableEntrySlotIndex)
{
    virtualTableSlotIndex = newVirtualTableSlotIndex;
    virtualTableEntrySlotIndex = newVirtualTableEntrySlotIndex;
}

uint32_t SpecificMethod::getVirtualTableSlotIndex() const
{
    return virtualTableSlotIndex;
}

uint32_t SpecificMethod::getVirtualTableEntrySlotIndex() const
{
    return virtualTableEntrySlotIndex;
}

bool SpecificMethod::canOverloadBinding(const AnyValuePtr &existentBinding) const
{
    return validAnyValue(existentBinding)->isMethod();
}

ProgramEntityPtr SpecificMethod::makeOverloadedBindingWith(const AnyValuePtr &existentBinding)
{
    sysmelAssert(existentBinding);
    if(existentBinding->isPatternMatchingMethod())
    {
        auto existentPatternMethod = staticObjectCast<PatternMatchingMethod> (existentBinding);
        existentPatternMethod->addPattern(selfFromThis());
        return existentPatternMethod;
    }

    sysmelAssert(existentBinding->isSpecificMethod());
    auto patternMatchingMethod = basicMakeObject<PatternMatchingMethod> (getName());
    patternMatchingMethod->addPattern(staticObjectCast<Method> (existentBinding));
    patternMatchingMethod->addPattern(selfFromThis());
    return patternMatchingMethod;
}


TypePtr SpecificMethod::adjustMacroInvocationSelfType(const TypePtr &selfType)
{
    auto parentEntity = getParentProgramEntity();
    if(validAnyValue(parentEntity)->isType())
    {
        auto parentType = parentEntity.staticAs<Type> ();
        if(parentType->isSubtypeOf(ReferenceTypeValue::__staticType__())
            || parentType->isSubtypeOf(TemporaryReferenceType::__staticType__()))
        return selfType;
    }
    return selfType->asDecayedType();
}

} // End of namespace Environment
} // End of namespace Sysmel