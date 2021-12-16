#include "Environment/Type.hpp"

#include "Environment/MethodDictionary.hpp"
#include "Environment/MessageNotUnderstood.hpp"
#include "Environment/ASTCallNode.hpp"
#include "Environment/ASTMessageSendNode.hpp"
#include "Environment/ASTLiteralValueNode.hpp"
#include "Environment/ASTSemanticAnalyzer.hpp"
#include "Environment/ASTProgramEntityExtensionNode.hpp"
#include "Environment/ASTMessageChainNode.hpp"
#include "Environment/ASTMessageChainMessageNode.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/Utilities.hpp"
#include "Environment/TypeConversionRule.hpp"
#include "Environment/IdentityTypeConversionRule.hpp"
#include "Environment/UpcastTypeConversionRule.hpp"
#include "Environment/DowncastTypeConversionRule.hpp"
#include "Environment/ValueAsVoidTypeConversionRule.hpp"
#include "Environment/ConstructorMethodTypeConversionRule.hpp"
#include "Environment/ConversionMethodTypeConversionRule.hpp"
#include "Environment/DeferredCompileTimeCodeFragment.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/UnsupportedOperation.hpp"
#include "Environment/SpecificMethod.hpp"
#include "Environment/FieldVariable.hpp"

#include "Environment/DecoratedType.hpp"
#include "Environment/PointerType.hpp"
#include "Environment/ReferenceType.hpp"
#include "Environment/TemporaryReferenceType.hpp"
#include "Environment/ArrayType.hpp"
#include "Environment/TupleType.hpp"
#include "Environment/VariantType.hpp"
#include "Environment/FunctionType.hpp"

#include "Environment/SSATypeProgramEntity.hpp"

#include "Environment/TypeVisitor.hpp"
#include "Environment/LiteralValueVisitor.hpp"

#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<Type> typeTypeRegistration;

MethodCategories Type::__instanceMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding("supertype", &Type::getSupertype, MethodFlags::Pure),
            makeMethodBinding("subtypes", &Type::getSubtypes, MethodFlags::Pure),

            makeMethodBinding<UIntPointerValue (const TypePtr &)> ("alignedMemorySize", +[](const TypePtr &self){
                return UIntPointerValue{self->getAlignedMemorySize()};
            }, MethodFlags::Pure),
            makeMethodBinding<UIntPointerValue (const TypePtr &)> ("memorySize", +[](const TypePtr &self){
                return UIntPointerValue{self->getMemorySize()};
            }, MethodFlags::Pure),
            makeMethodBinding<UIntPointerValue (const TypePtr &)> ("memoryAlignment", +[](const TypePtr &self){
                return UIntPointerValue{self->getMemoryAlignment()};
            }, MethodFlags::Pure),

            makeMethodBinding<UIntPointerValue (const TypePtr &)> ("alignedValueSize", +[](const TypePtr &self){
                return UIntPointerValue{self->getAlignedValueSize()};
            }, MethodFlags::Pure),
            makeMethodBinding<UIntPointerValue (const TypePtr &)> ("valueSize", +[](const TypePtr &self){
                return UIntPointerValue{self->getValueSize()};
            }, MethodFlags::Pure),
            makeMethodBinding<UIntPointerValue (const TypePtr &)> ("valueAlignment", +[](const TypePtr &self){
                return UIntPointerValue{self->getValueAlignment()};
            }, MethodFlags::Pure),
        }},

        {"type composition", {
            makeMethodBinding("array", &Type::arrayWithoutSize, MethodFlags::Pure),
            makeMethodBinding("array:", &Type::arrayWithSize, MethodFlags::Pure),

            makeMethodBinding("pointer", &Type::pointer, MethodFlags::Pure),
            makeMethodBinding("pointerFor:", &Type::pointerFor, MethodFlags::Pure),
            makeMethodBinding("ref", &Type::ref, MethodFlags::Pure),
            makeMethodBinding("refFor:", &Type::refFor, MethodFlags::Pure),
            makeMethodBinding("tempRef", &Type::tempRef, MethodFlags::Pure),
            makeMethodBinding("tempRefFor:", &Type::tempRefFor, MethodFlags::Pure),

            makeMethodBinding("const", &Type::withConst, MethodFlags::Pure),
            makeMethodBinding("restrict", &Type::withRestrict, MethodFlags::Pure),
            makeMethodBinding("volatile", &Type::withVolatile, MethodFlags::Pure),

            makeMethodBinding("&", &Type::appendTypeMakingTuple, MethodFlags::Pure),
            makeMethodBinding("|", &Type::appendTypeMakingVariant, MethodFlags::Pure),
            makeMethodBinding("=>", &Type::appendResultTypeMakingFunctionType, MethodFlags::Pure),
        }},
    };
}

TypePtr Type::extractTypeForTypeMacroReceiverNode(const ASTNodePtr &receiverNode)
{
    assert(receiverNode->analyzedType);

    if(receiverNode->isASTLiteralTypeNode())
    {
        auto typeValue = receiverNode.staticAs<ASTLiteralValueNode> ()->value;
        assert(typeValue && typeValue->isType());
        return staticObjectCast<Type> (typeValue);
    }

    return receiverNode->analyzedType->getInstanceType();
}

MethodCategories Type::__instanceMacroMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr)> ("extend:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &bodyNode) {
                auto extensionNode = basicMakeObject<ASTProgramEntityExtensionNode> ();
                extensionNode->programEntity = macroContext->receiverNode;
                extensionNode->body = bodyNode;
                return extensionNode;
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr)> ("definition:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &bodyNode) {
                auto extensionNode = basicMakeObject<ASTProgramEntityExtensionNode> ();
                extensionNode->programEntity = macroContext->receiverNode;
                extensionNode->body = bodyNode;
                return extensionNode;
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("basicNewValue", [](const MacroInvocationContextPtr &macroContext) {
                return extractTypeForTypeMacroReceiverNode(macroContext->receiverNode)->expandBasicNewValue(macroContext);
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("newValue", [](const MacroInvocationContextPtr &macroContext) {
                return extractTypeForTypeMacroReceiverNode(macroContext->receiverNode)->expandNewValue(macroContext);
            }, MethodFlags::Macro),
        }},
    };
}

bool Type::isUndefinedType() const
{
    return isSubtypeOf(getUndefinedType());
}

bool Type::isAutoType() const
{
    return this == getAutoType().get();
}

bool Type::isVoidType() const
{
    return isSubtypeOf(getVoidType());
}

bool Type::isControlFlowEscapeType() const
{
    return isSubtypeOf(getControlFlowEscapeType());
}

bool Type::isReturnType() const
{
    return isSubtypeOf(getReturnType());
}

bool Type::isBreakType() const
{
    return isSubtypeOf(getBreakType());
}

bool Type::isContinueType() const
{
    return isSubtypeOf(getContinueType());
}

bool Type::isLiteralValueType() const
{
    return isSubtypeOf(getLiteralValueType());
}

bool Type::isCompilationErrorValueType() const
{
    return isSubtypeOf(getCompilationErrorValueType());
}

bool Type::isType() const
{
    return true;
}

AnyValuePtr Type::getName() const
{
    return name;
}

void Type::setName(const AnyValuePtr &newName)
{
    name = newName;
}

std::string Type::printString() const
{
    if(name)
        return name->asString();
    return SuperType::printString();
}

AnyValuePtr Type::acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor)
{
    return visitor->visitType(selfFromThis());
}

AnyValuePtr Type::acceptTypeVisitor(const TypeVisitorPtr &visitor)
{
    return visitor->visitAnyType(selfFromThis());
}

TypePtr Type::getSupertype() const
{
    return supertype;
}

void Type::setSupertype(const TypePtr &newSupertype)
{
    supertype = newSupertype;
}

const TypePtrList &Type::getSubtypes()
{
    return subtypes;
}

void Type::registerSubtype(const TypePtr &subtype)
{
    if(!subtype->isMetaType())
        subtypes.push_back(subtype);
}

TypePtr Type::asConstReceiverType()
{
    return selfFromThis();
}

TypePtr Type::asReceiverType()
{
    return selfFromThis();
}

AnyValuePtr Type::lookupLocalSelector(const AnyValuePtr &selector)
{
    if(isMetaType())
        getInstanceType()->evaluateAllPendingBodyBlockCodeFragments();
    evaluateAllPendingBodyBlockCodeFragments();
    return methodDictionary ? methodDictionary->lookupSelector(selector) : AnyValuePtr();
}

AnyValuePtr Type::lookupLocalMacroSelector(const AnyValuePtr &selector)
{
    if(isMetaType())
        getInstanceType()->evaluateAllPendingBodyBlockCodeFragments();
    evaluateAllPendingBodyBlockCodeFragments();
    return macroMethodDictionary ? macroMethodDictionary->lookupSelector(selector) : AnyValuePtr();
}

AnyValuePtr Type::lookupLocalMacroFallbackSelector(const AnyValuePtr &selector)
{
    if(isMetaType())
        getInstanceType()->evaluateAllPendingBodyBlockCodeFragments();
    evaluateAllPendingBodyBlockCodeFragments();
    return macroFallbackMethodDictionary ? macroFallbackMethodDictionary->lookupSelector(selector) : AnyValuePtr();
}

AnyValuePtr Type::lookupMacroSelector(const AnyValuePtr &selector)
{
    auto localMethod = lookupLocalMacroSelector(selector);
    if(localMethod)
        return localMethod;

    auto superType = getSupertype();
    if(superType)
        return superType->lookupMacroSelector(selector);

    return AnyValuePtr();
}

AnyValuePtr Type::lookupSelector(const AnyValuePtr &selector)
{
    auto localMethod = lookupLocalSelector(selector);
    if(localMethod)
        return localMethod;

    auto superType = getSupertype();
    if(superType)
        return superType->lookupSelector(selector);

    return AnyValuePtr();
}

AnyValuePtr Type::lookupMacroFallbackSelector(const AnyValuePtr &selector)
{
    auto localMethod = lookupLocalMacroFallbackSelector(selector);
    if(localMethod)
        return localMethod;

    auto superType = getSupertype();
    if(superType)
        return superType->lookupMacroFallbackSelector(selector);

    return AnyValuePtr();
}

AnyValuePtr Type::lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    (void)accessingScope;
    auto it = bindings.find(symbol);
    if(it == bindings.end())
        return nullptr;

    return it->second.second;
}

ASTNodePtr Type::analyzeCallNode(const ASTCallNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    assert(node->function && node->function->analyzedType);

    // In case the fuction is an error, analyze the arguments to
    // further discover more error, and then return the receiver to propagate an error node.
    if(isCompilationErrorValueType())
    {
        assert(node->function->analyzedType->isCompilationErrorValueType());
        for(const auto &arg : node->arguments)
            semanticAnalyzer->analyzeNodeIfNeededWithTemporaryAutoType(arg);
        return node->function;
    }

    // Allow the actual function to analyze the call. This is typically used by metabuilders.
    if(supportsMessageAnalysisByLiteralValueReceivers() && node->function->isASTLiteralValueNode())
    {
        auto literalValue = node->function.staticAs<ASTLiteralValueNode> ()->value;
        return literalValue->analyzeCallNode(node, semanticAnalyzer);
    }

    // By default, convert the call into a message send, and then analyze back.
    return semanticAnalyzer->analyzeCallNodeByConvertingToMessageSend(node);
}

ASTNodePtr Type::analyzeMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    assert(node->receiver && node->receiver->analyzedType);
    assert(node->selector && node->selector->analyzedType);

    // Allow the actual instance to analyze the message. This is typically used by metabuilders.
    if(supportsMessageAnalysisByLiteralValueReceivers() && node->receiver->isASTLiteralValueNode())
    {
        auto literalValue = staticObjectCast<ASTLiteralValueNode> (node->receiver)->value;
        return literalValue->analyzeMessageSendNode(node, semanticAnalyzer);
    }

    return analyzeMessageSendNodeWithTypeDefinedMethods(node, semanticAnalyzer);
}

ASTNodePtr Type::analyzeMessageSendNodeWithTypeDefinedMethods(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    assert(node->receiver && node->receiver->analyzedType);
    assert(node->selector && node->selector->analyzedType);

    // In case the receiver is an error, analyze the arguments to
    // further discover more error, and then return the receiver to propagate an error node.
    if(isCompilationErrorValueType())
    {
        assert(node->receiver->analyzedType->isCompilationErrorValueType());
        for(const auto &arg : node->arguments)
            semanticAnalyzer->analyzeNodeIfNeededWithTemporaryAutoType(arg);
        return node->receiver;
    }

    AnyValuePtr directSelectorValue;
    if(node->selector->isASTLiteralValueNode())
        directSelectorValue = node->selector.staticAs<ASTLiteralValueNode> ()->value;

    // Attempt going through the different expansion levels
    auto startExpansionLevel = node->expansionLevel;
    while(uint8_t(node->expansionLevel) < uint8_t(MessageSendExpansionLevel::Count))
    {
        if(directSelectorValue)
        {
            auto macroOrMethod = lookupSelectorInReceiverNodeWithExpansionLevel(directSelectorValue, node->receiver, node->expansionLevel);
            if(macroOrMethod)
                return macroOrMethod->analyzeMessageSendNode(node, semanticAnalyzer);
        }

        node->expansionLevel = MessageSendExpansionLevel(uint8_t(node->expansionLevel) + 1);
    }

    node->expansionLevel = startExpansionLevel;
    return analyzeUnboundMessageSendNode(node, semanticAnalyzer);
}

bool Type::supportsDynamicCompileTimeMessageSend() const
{
    return false;
}

bool Type::supportsMessageAnalysisByLiteralValueReceivers() const
{
    return false;
}

bool Type::isEphemeralCompileTimeObject() const
{
    return false;
}

bool Type::isNullableType() const
{
    return true;
}

bool Type::isImmutableType()
{
    return false;
}

bool Type::isGCType()
{
    return true;
}

bool Type::isSeparateValueInstanceType()
{
    return isGCType();
}

bool Type::isPassedByReference()
{
    return false;
}

bool Type::isReturnedByReference()
{
    return false;
}

bool Type::hasTrivialInitialization()
{
    return false;
}

bool Type::hasTrivialInitializationCopyingFrom()
{
    return false;
}

bool Type::hasTrivialInitializationMovingFrom()
{
    return false;
}

bool Type::hasTrivialFinalization()
{
    return false;
}

bool Type::hasTrivialCopyingFrom()
{
    return false;
}

bool Type::hasTrivialMovingFrom()
{
    return false;
}
uint64_t Type::getValueSize()
{
    return getMemorySize();
}

uint64_t Type::getValueAlignment()
{
    return getMemoryAlignment();
}

uint64_t Type::getAlignedValueSize()
{
    return getAlignedMemorySize();
}

uint64_t Type::getMemorySize()
{
    return 0;
}

uint64_t Type::getMemoryAlignment()
{
    return 0;
}

uint64_t Type::getAlignedMemorySize()
{
    return alignedTo(getMemorySize(), getMemoryAlignment());
}

ASTNodePtr Type::analyzeUnboundMessageSendNode(const ASTMessageSendNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(supportsDynamicCompileTimeMessageSend())
        return semanticAnalyzer->analyzeDynamicCompileTimeMessageSendNode(node);

    // Support the does not understand macro.
    auto dnuMacro = lookupDoesNotUnderstandMacro();
    if(dnuMacro)
        return semanticAnalyzer->analyzeMessageSendNodeViaDNUMacro(node, dnuMacro);

    if(node->selector->isASTLiteralSymbolValue())
    {
        auto directSelectorValue = staticObjectCast<ASTLiteralValueNode> (node->selector)->value;
        return semanticAnalyzer->recordSemanticErrorInNode(node, formatString("Failed to find matching message for selector {0} in receiver of type '{1}'.", {{directSelectorValue->printString(), printString()}}));
    }
    
    return semanticAnalyzer->recordSemanticErrorInNode(node, "");
}

ASTNodePtr Type::concretizeEphemeralCompileTimeObject(const ASTLiteralValueNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    if(supportsMessageAnalysisByLiteralValueReceivers())
        return node->value->concretizeEphemeralCompileTimeObject(node, semanticAnalyzer);
    return SuperType::concretizeEphemeralCompileTimeObject(node, semanticAnalyzer);
}

AnyValuePtr Type::lookupDoesNotUnderstandMacro()
{
    return nullptr;
}

void Type::addConstructor(const AnyValuePtr &constructorMethod)
{
    if(!constructorMethod->isSpecificMethod())
        signalNewWithMessage<UnsupportedOperation> ("Cannot add a non-specific method as a constructor in a type.");

    constructors.push_back(staticObjectCast<SpecificMethod> (constructorMethod));
}

void Type::addConstructors(const AnyValuePtrList &constructorMethods)
{
    for(auto &ctor : constructorMethods )
        addConstructor(ctor);
}

void Type::addConversion(const AnyValuePtr &conversionMethod)
{
    if(!conversionMethod->isSpecificMethod())
        signalNewWithMessage<UnsupportedOperation> ("Cannot add a non-specific method as a conversion in a type.");

    conversions.push_back(staticObjectCast<SpecificMethod> (conversionMethod));
}

void Type::addConversions(const AnyValuePtrList &conversionMethods)
{
    for(auto &conversion : conversionMethods )
        addConversion(conversion);
}

void Type::addFieldVariableWithVisibility(const FieldVariablePtr &field, ProgramEntityVisibility visibility)
{
    assert(!field->getParentProgramEntity());
    if(!canHaveUserDefinedFields() && !field->isBootstrapFieldVariable())
        signalNewWithMessage<UnsupportedOperation> (formatString("Cannot add an user defined field to type {0}.", {printString()}));

    recordChildProgramEntityDefinition(field);
    bindProgramEntityWithVisibility(field, visibility);
    fields.push_back(field);
}

size_t Type::getFieldCount()
{
    if(canHaveUserDefinedFields())
        ensureSemanticAnalysis();
    return fields.size();
}

const FieldVariablePtr &Type::getField(size_t index)
{
    if(canHaveUserDefinedFields())
        ensureSemanticAnalysis();
    return fields[index];
}

const FieldVariablePtrList &Type::getFields()
{
    if(canHaveUserDefinedFields())
        ensureSemanticAnalysis();
    return fields;
}

void Type::addMacroMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    if(!macroMethodDictionary)
        macroMethodDictionary = basicMakeObject<MethodDictionary> ();
    macroMethodDictionary->addMethodWithSelector(method, selector);
}

void Type::addMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    if(!methodDictionary)
        methodDictionary = basicMakeObject<MethodDictionary> ();
    methodDictionary->addMethodWithSelector(method, selector);
}

void Type::addMacroFallbackMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    if(!macroMethodDictionary)
        macroMethodDictionary = basicMakeObject<MethodDictionary> ();
    macroMethodDictionary->addMethodWithSelector(method, selector);
}

TypePtr Type::getInstanceType()
{
    return selfFromThis();
}

TypePtr Type::getMetaType()
{
    return getType();
}

bool Type::isSubtypeOf(const TypePtr &otherType) const
{
    if(this == otherType.get())
        return true;
    
    auto supertype = getSupertype();
    if(supertype)
        return supertype->isSubtypeOf(otherType);

    return false;
}

PatternMatchingRank Type::rankToMatchType(const TypePtr &type)
{
    PatternMatchingRank rank = 0;
    for(auto currentType = type; currentType; currentType = currentType->getSupertype())
    {
        if(currentType.get() == this)
            return rank;

        ++rank;
    }

    return -1;
}

PatternMatchingRank Type::rankToMatchValue(const AnyValuePtr &value)
{
    return rankToMatchType(value->getType());
}

AnyValuePtr Type::runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
{
    auto method = lookupSelector(selector);
    if(method)
        return method->runWithArgumentsIn(selector, arguments, receiver);

    signalNewWithMessage<MessageNotUnderstood> ("Message " + selector->printString() + " is not understood by " + receiver->printString() + ".");
}

void Type::subtypesDo(const TypeIterationBlock &aBlock)
{
    for(auto &subtype : subtypes)
        aBlock(subtype);
}

void Type::allSubtypesDo(const TypeIterationBlock &aBlock)
{
    TypeIterationBlock recursiveBlock = [&](const TypePtr &type) {
        aBlock(type);
        type->subtypesDo(recursiveBlock);
    };

    subtypesDo(recursiveBlock);
}

void Type::withAllSubtypesDo(const TypeIterationBlock &aBlock)
{
    aBlock(selfFromThis());
    allSubtypesDo(aBlock);
}

void Type::ensureSemanticAnalysis()
{
    SuperType::ensureSemanticAnalysis();
    evaluateAllPendingCodeFragments();
}

void Type::evaluateAllPendingCodeFragments()
{
    evaluateAllPendingBodyBlockCodeFragments();
}

void Type::evaluateAllPendingBodyBlockCodeFragments()
{
    while(!pendingBodyBlockCodeFragments.empty())
    {
        auto fragmentsToProcess = pendingBodyBlockCodeFragments;
        pendingBodyBlockCodeFragments.clear();
        for(auto &fragment : fragmentsToProcess)
        {
            fragment->analyzeAndEvaluate();
        }
    }
}

void Type::enqueuePendingBodyBlockCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment)
{
    pendingBodyBlockCodeFragments.push_back(codeFragment);
    enqueuePendingSemanticAnalysis();
}

void Type::recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild)
{
    auto oldParent = newChild->getParentProgramEntity();
    assert(!oldParent || oldParent == selfFromThis());
    if(oldParent)
        return;

    children.push_back(newChild);
    newChild->setParentProgramEntity(selfFromThis());
}

void Type::bindSymbolWithVisibility(const AnyValuePtr &symbol, ProgramEntityVisibility visibility, const ProgramEntityPtr &binding)
{
    assert(symbol && !symbol->isAnonymousNameSymbol());
    if(bindings.find(symbol) != bindings.end())
        signalNewWithMessage<Error> ("Expected a new symbol binding.");

    bindings[symbol] = std::make_pair(visibility, binding);
    
    if(symbol->isLiteralSymbol() && visibility == ProgramEntityVisibility::Public)
    {
        if(isMetaType())
        {
            if(!binding->isFieldVariable())
                binding->addPublicAccessingMethodsWithSymbolOnto(symbol, selfFromThis());
            binding->addPublicInstanceAccessingMethodsWithSymbolOnto(symbol, selfFromThis());
        }
        else
        {
            binding->addPublicAccessingMethodsWithSymbolOnto(symbol, getType());
            binding->addPublicInstanceAccessingMethodsWithSymbolOnto(symbol, selfFromThis());
        }
    }
}

void Type::addDefaultTypeConversionRules()
{
    addTypeConversionRule(IdentityTypeConversionRule::uniqueInstance());
    addTypeConversionRule(UpcastTypeConversionRule::uniqueInstance());
    addTypeConversionRule(ValueAsVoidTypeConversionRule::uniqueInstance());
    addExplicitTypeConversionRule(DowncastTypeConversionRule::uniqueInstance());
}

void Type::addTypeConversionRule(const TypeConversionRulePtr &rule)
{
    addExplicitTypeConversionRule(rule);
    addImplicitTypeConversionRule(rule);
}

void Type::addExplicitTypeConversionRule(const TypeConversionRulePtr &rule)
{
    explicitTypeConversionRules.push_back(rule);
}

void Type::addImplicitTypeConversionRule(const TypeConversionRulePtr &rule)
{
    implicitTypeConversionRules.push_back(rule);
}

void Type::addReinterpretTypeConversionRule(const TypeConversionRulePtr &rule)
{
    reinterpretTypeConversionRules.push_back(rule);
}

TypeConversionRulePtr Type::findImplicitTypeConversionRuleForInto(const ASTNodePtr &node, const TypePtr &targetType)
{
    auto sourceType = selfFromThis();
    for(auto & rule : implicitTypeConversionRules)
    {
        if(rule->canBeUsedToConvertNodeFromTo(node, sourceType, targetType))
            return rule;
    }

    // Find a conversion method here.
    for(auto &conversion : conversions)
    {
        if(conversion->isExplicit())
            continue;

        auto conversionTargetType = conversion->getFunctionalType()->getResultType();
        if(conversionTargetType == targetType)
            return ConversionMethodTypeConversionRule::makeFor(sourceType, conversionTargetType, conversion);
    }

    // Find a constructor method in the target.
    for(auto &targetConstructor : targetType->constructors)
    {
        const auto &constructorType = targetConstructor->getFunctionalType();
        if(constructorType->getArgumentCount() != 1)
            continue;

        if(targetConstructor->isExplicit())
            continue;

        const auto &constructorSourceType = constructorType->getArgument(0);
        if(sourceType == constructorSourceType)
            return ConstructorMethodTypeConversionRule::makeFor(sourceType, targetType, targetConstructor);
    }

    return nullptr;
}

TypeConversionRulePtr Type::findExplicitTypeConversionRuleForInto(const ASTNodePtr &node, const TypePtr &targetType)
{
    auto sourceType = selfFromThis();
    for(auto & rule : explicitTypeConversionRules)
    {
        if(rule->canBeUsedToConvertNodeFromTo(node, sourceType, targetType))
            return rule;
    }

    // Find a conversion method here.
    for(auto &conversion : conversions)
    {
        auto conversionTargetType = conversion->getFunctionalType()->getResultType();
        if(conversionTargetType == targetType)
            return ConversionMethodTypeConversionRule::makeFor(sourceType, conversionTargetType, conversion);
    }

    // Find a constructor method in the target.
    for(auto &targetConstructor : targetType->constructors)
    {
        const auto &constructorType = targetConstructor->getFunctionalType();
        if(constructorType->getArgumentCount() != 1)
            continue;

        const auto &constructorSourceType = constructorType->getArgument(0);
        if(sourceType == constructorSourceType)
            return ConstructorMethodTypeConversionRule::makeFor(sourceType, targetType, targetConstructor);
    }
    
    return nullptr;
}

TypeConversionRulePtr Type::findReinterpretTypeConversionRuleForInto(const ASTNodePtr &node, const TypePtr &targetType)
{
    auto sourceType = selfFromThis();
    for(auto & rule : reinterpretTypeConversionRules)
    {
        if(rule->canBeUsedToConvertNodeFromTo(node, sourceType, targetType))
            return rule;
    }

    return nullptr;
}

AnyValuePtr Type::basicNewValue()
{
    return nullptr;
}

AnyValuePtr Type::shallowCloneInstance(const AnyValuePtr &cloned)
{
    (void)cloned;
    return nullptr;
}

AnyValuePtr Type::defaultValue()
{
    return isNullableType() ? nullptr : basicNewValue();
}

ASTNodePtr Type::expandBasicNewValue(const MacroInvocationContextPtr &context)
{
    return validAnyValue(basicNewValue())->asASTNodeRequiredInPosition(context->astBuilder->sourcePosition);
}

ASTNodePtr Type::expandNewValue(const MacroInvocationContextPtr &context)
{
    auto basicNewValue = expandBasicNewValue(context);
    if(hasTrivialInitialization())
        return basicNewValue;

    auto &builder = context->astBuilder;
    return builder->messageChain(basicNewValue, {
        builder->messageChainMessage(builder->literalSymbol("initialize"), {}),
        builder->messageChainMessage(builder->literalSymbol("yourself"), {})
    });
}

ASTNodePtr Type::expandCopyConstruction(const MacroInvocationContextPtr &context, const ASTNodePtr &valueNode)
{
    (void)context;
    if(isImmutableType())
        return valueNode;

    assert("TODO: expandCopyConstruction" && false);
}

ASTNodePtr Type::expandMoveConstruction(const MacroInvocationContextPtr &context, const ASTNodePtr &valueNode)
{
    (void)context;
    if(isImmutableType())
        return valueNode;

    assert("TODO: expandMoveConstruction" && false);
}

ASTNodePtr Type::analyzeValueConstructionWithArguments(const ASTNodePtr &node, const ASTNodePtrList &arguments, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    ensureSemanticAnalysis();
    auto analyzedArguments = arguments;

    // Ensure the arguments are analyzed.
    for(auto &arg : analyzedArguments)
        arg = semanticAnalyzer->analyzeNodeIfNeededWithTemporaryAutoType(arg);

    // Special single argument constructors.
    if(analyzedArguments.size() == 1)
    {
        auto argument = analyzedArguments.front();
        auto argumentType = argument->analyzedType;

        // Copy constructor.
        if(argumentType == selfFromThis())
            return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(
                expandCopyConstruction(semanticAnalyzer->makeMacroInvocationContextFor(node), argument)
            );
            
        // TODO: move constructor
    }

    // Do we have some constructors?
    if(!constructors.empty())
    {
        // Make a synthetic message send node.
        auto messageSendNode = basicMakeObject<ASTMessageSendNode> ();
        messageSendNode->sourcePosition = node->sourcePosition;
        messageSendNode->selector = internSymbol("()")->asASTNodeRequiredInPosition(node->sourcePosition);
        messageSendNode->receiver = asASTNodeRequiredInPosition(node->sourcePosition);
        messageSendNode->receiver->analyzedType = getType();
        messageSendNode->arguments = analyzedArguments;

        // Find a matching constructor.
        std::vector<MethodPtr> matchingCandidates;
        PatternMatchingRank bestRank = std::numeric_limits<PatternMatchingRank>::max();

        for(auto &constructor : constructors)
        {
            auto result = constructor->matchPatternForAnalyzingMessageSendNode(messageSendNode, semanticAnalyzer);
            if(!result.matchingMethod)
                continue;

            if(result.matchingRank < bestRank)
            {
                matchingCandidates.clear();
                matchingCandidates.push_back(result.matchingMethod);
                bestRank = result.matchingRank;
            }
            else if(result.matchingRank == bestRank)
            {
                matchingCandidates.push_back(result.matchingMethod);
            }
        }

        if(matchingCandidates.size() == 1)
        {
            return matchingCandidates.front()->analyzeMessageSendNode(messageSendNode, semanticAnalyzer);
        }
        else if(matchingCandidates.size() > 1)
        {
            return semanticAnalyzer->recordSemanticErrorInNode(node, formatString("Ambiguous matching constructors of type {0} using the specified arguments.", {printString()}));
        }
    }

    // If no arguments are given, default to expand new value.
    if(arguments.empty())
    {
        return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(
            expandNewValue(semanticAnalyzer->makeMacroInvocationContextFor(node))
        );
    }

    return semanticAnalyzer->recordSemanticErrorInNode(node, formatString("Unsupported construction of value with type {0} using the specified arguments.", {printString()}));
}

bool Type::canBeInstantiatedWithLiteralValue(const AnyValuePtr &value)
{
    (void)value;
    return false;
}

AnyValuePtr Type::instantiatedWithLiteralValue(const AnyValuePtr &value)
{
    (void)value;
    return nullptr;
}

TypePtr Type::asInferredTypeForWithModeInEnvironment(const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment)
{
    (void)node;
    (void)mode;
    (void)isMutable;
    (void)concreteLiterals;
    (void)environment;
    return selfFromThis();
}

PointerTypePtr Type::pointer()
{
    return PointerType::make(selfFromThis());
}

PointerTypePtr Type::pointerFor(const AnyValuePtr &addressSpace)
{
    return PointerType::makeWithAddressSpace(selfFromThis(), addressSpace);
}

ReferenceTypePtr Type::ref()
{
    return ReferenceType::make(selfFromThis());
}

ReferenceTypePtr Type::refFor(const AnyValuePtr &addressSpace)
{
    return ReferenceType::makeWithAddressSpace(selfFromThis(), addressSpace);
}

PointerLikeTypePtr Type::tempRef()
{
    return TemporaryReferenceType::make(selfFromThis());
}

PointerLikeTypePtr Type::tempRefFor(const AnyValuePtr &addressSpace)
{
    return TemporaryReferenceType::makeWithAddressSpace(selfFromThis(), addressSpace);
}

ArrayTypePtr Type::arrayWithoutSize()
{
    return arrayWithSize(0);
}

ArrayTypePtr Type::arrayWithSize(uint64_t size)
{
    return ArrayType::make(selfFromThis(), size);
}

TypePtr Type::asTupleType()
{
    return TupleType::make({selfFromThis()});
}

TypePtr Type::appendTypeMakingTuple(const TypePtr &nextType)
{
    TypePtrList newElementTypes;
    newElementTypes.push_back(selfFromThis());

    if(nextType->isTupleType())
    {
        auto &nextTypes = nextType.staticAs<TupleType> ()->elementTypes;
        newElementTypes.insert(newElementTypes.end(), nextTypes.begin(), nextTypes.end());
    }
    else
    {
        newElementTypes.push_back(nextType);
    }

    return TupleType::make(newElementTypes);
}

TypePtr Type::appendTypeMakingVariant(const TypePtr &nextType)
{
    TypePtrList newElementTypes;
    newElementTypes.push_back(selfFromThis());

    if(nextType->isVariantType())
    {
        auto &nextTypes = staticObjectCast<VariantType> (nextType)->elementTypes;
        newElementTypes.insert(newElementTypes.end(), nextTypes.begin(), nextTypes.end());
    }
    else
    {
        newElementTypes.push_back(nextType);
    }

    return VariantType::make(newElementTypes);
}

TypePtr Type::appendResultTypeMakingFunctionType(const TypePtr &resultType)
{
    if(isVoidType())
        return FunctionType::make(resultType, {});
    else
        return FunctionType::make(resultType, {selfFromThis()});
}

TypePtr Type::withConst()
{
    return withDecorations(TypeDecorationFlags::Const);
}

TypePtr Type::withRestrict()
{
    return withDecorations(TypeDecorationFlags::Restrict);
}

TypePtr Type::withVolatile()
{
    return withDecorations(TypeDecorationFlags::Volatile);
}

TypePtr Type::withDecorations(TypeDecorationFlags decorations)
{
    return DecoratedType::make(selfFromThis(), decorations);
}

TypePtr Type::asUndecoratedType()
{
    return selfFromThis();
}

bool Type::isConstOrConstReferenceType() const
{
    return isConstDecoratedType();
}

TypePtr Type::asConstOrConstReferenceType()
{
    return withConst();
}

SSAValuePtr Type::asSSAValueRequiredInPosition(const ASTSourcePositionPtr &)
{
    if(!ssaTypeProgramEntity)
    {
        ssaTypeProgramEntity = basicMakeObject<SSATypeProgramEntity> ();
        ssaTypeProgramEntity->setValue(selfFromThis());
        auto parentProgramEntity = getParentProgramEntity();
        if(parentProgramEntity)
        {
            auto parentSSAValue = parentProgramEntity->asProgramEntitySSAValue();
            if(parentSSAValue->isSSAProgramEntity())
            {
                assert(parentSSAValue->isSSAProgramEntity());
                parentSSAValue.staticAs<SSAProgramEntity> ()->addChild(ssaTypeProgramEntity);
            }
        }
        else
        {
            // TODO: Handle the bootstrap environment defined types.
        }
    }

    return ssaTypeProgramEntity;
}

} // End of namespace Environment
} // End of namespace Sysmel