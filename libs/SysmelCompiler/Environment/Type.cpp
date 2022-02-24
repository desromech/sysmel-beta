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
#include "Environment/ASTSourcePosition.hpp"
#include "Environment/ASTBuilder.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"
#include "Environment/BootstrapMethod.hpp"
#include "Environment/StringUtilities.hpp"
#include "Environment/Utilities.hpp"
#include "Environment/TypeConversionRule.hpp"
#include "Environment/ChainedTypeConversionRule.hpp"
#include "Environment/IdentityTypeConversionRule.hpp"
#include "Environment/UpcastTypeConversionRule.hpp"
#include "Environment/DowncastTypeConversionRule.hpp"
#include "Environment/ValueAsReceiverReferenceTypeConversionRule.hpp"
#include "Environment/ValueAsVoidTypeConversionRule.hpp"
#include "Environment/ValueAsConstRefTypeConversionRule.hpp"
#include "Environment/ValueInVariantTypeConversionRule.hpp"
#include "Environment/ConstructorMethodTypeConversionRule.hpp"
#include "Environment/ConversionMethodTypeConversionRule.hpp"
#include "Environment/DeferredCompileTimeCodeFragment.hpp"
#include "Environment/MacroInvocationContext.hpp"
#include "Environment/UnsupportedOperation.hpp"
#include "Environment/SpecificMethod.hpp"
#include "Environment/FieldVariable.hpp"

#include "Environment/IdentifierLookupScope.hpp"

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

#include "Environment/RuntimeContext.hpp"

#include "Environment/SubclassResponsibility.hpp"

#include <queue>
#include <algorithm>

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<Type> typeTypeRegistration;

Type::Type()
{
    definitionPosition = ASTSourcePosition::empty();
}

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

        {"pattern matching", {
            makeMethodBinding<bool (const TypePtr &)> ("isSequencePatternType", +[](const TypePtr &){
                return false;
            }, MethodFlags::Pure),

            makeMethodBinding<uint64_t (const TypePtr &)> ("sequencePatternTypeMinSize", +[](const TypePtr &){
                return 0;
            }, MethodFlags::Pure),
            makeMethodBinding<uint64_t (const TypePtr &)> ("sequencePatternTypeMaxSize", +[](const TypePtr &){
                return 0;
            }, MethodFlags::Pure),
            makeMethodBinding<TypePtr (const TypePtr &, uint64_t)> ("sequencePatternTypeAt:", +[](const TypePtr &, uint64_t){
                return TypePtr();
            }, MethodFlags::Pure),
        }},

        {"type composition", {
            makeMethodBinding("array", &Type::arrayWithoutSize, MethodFlags::Pure),
            makeMethodBinding("array:", &Type::arrayWithSize, MethodFlags::Pure),
            makeMethodBinding("[]", &Type::arrayWithoutSize, MethodFlags::Pure),
            makeMethodBinding("[]", &Type::arrayWithSize, MethodFlags::Pure),

            makeMethodBinding("pointer", &Type::pointer, MethodFlags::Pure),
            makeMethodBinding("pointerFor:", &Type::pointerFor, MethodFlags::Pure),
            makeMethodBinding("ref", &Type::ref, MethodFlags::Pure),
            makeMethodBinding("refFor:", &Type::refFor, MethodFlags::Pure),
            makeMethodBinding("tempRef", &Type::tempRef, MethodFlags::Pure),
            makeMethodBinding("tempRefFor:", &Type::tempRefFor, MethodFlags::Pure),

            makeMethodBinding("const", &Type::withConst, MethodFlags::Pure),
            makeMethodBinding("params", &Type::withParams, MethodFlags::Pure),
            makeMethodBinding("restrict", &Type::withRestrict, MethodFlags::Pure),
            makeMethodBinding("volatile", &Type::withVolatile, MethodFlags::Pure),

            makeMethodBinding("&", &Type::appendTypeMakingTuple, MethodFlags::Pure),
            makeMethodBinding("|", &Type::appendTypeMakingVariant, MethodFlags::Pure),
            makeMethodBinding("=>", &Type::appendResultTypeMakingFunctionType, MethodFlags::Pure),
        }},

        {"type conversion", {
            makeMethodBinding("asDecayedType", &Type::asDecayedType, MethodFlags::Pure),
        }},

        {"testing", {
            makeMethodBinding("isConstDecoratedType", &Type::isConstDecoratedType, MethodFlags::Pure),
        }},
    };
}

TypePtr Type::extractTypeForTypeMacroReceiverNode(const ASTNodePtr &receiverNode)
{
    sysmelAssert(receiverNode->analyzedType);

    if(receiverNode->isASTLiteralTypeNode())
    {
        auto typeValue = receiverNode.staticAs<ASTLiteralValueNode> ()->value;
        sysmelAssert(typeValue && typeValue->isType());
        return staticObjectCast<Type> (typeValue);
    }

    return receiverNode->analyzedType->getInstanceType();
}

MethodCategories Type::__instanceMacroMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr)> ("extend:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &bodyNode) {
                return macroContext->astBuilder->programEntityExtension(macroContext->receiverNode, bodyNode);
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr)> ("definition:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &bodyNode) {
                return macroContext->astBuilder->programEntityExtension(macroContext->receiverNode, bodyNode);
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

TypePtr Type::computeConditionCoercionType(const TypePtr &first, const TypePtr &second)
{
    auto firstDecayed = first->asDecayedType();
    auto secondDecayed = second->asDecayedType();

    if(firstDecayed == secondDecayed)
        return firstDecayed;
    else if(firstDecayed->isControlFlowEscapeType() && secondDecayed->isControlFlowEscapeType())
        return getControlFlowEscapeType();
    else if(firstDecayed->isControlFlowEscapeType())
        return secondDecayed;
    else if(secondDecayed->isControlFlowEscapeType())
        return firstDecayed;
    else
        return getVoidType();
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

TypePtr Type::asSuperReceiverType()
{
    auto superType = getSupertype();
    return superType ? superType->asReceiverType() : nullptr;
}

AnyValuePtr Type::asSelfForStaticMethod()
{
    return getInstanceType();
}

AnyValuePtr Type::lookupLocalSelector(const AnyValuePtr &selector)
{
    if(isMetaType())
        getInstanceType()->evaluateAllPendingBodyBlockCodeFragments();
    evaluateAllPendingBodyBlockCodeFragments();

    if(validAnyValue(selector)->isLiteralSymbol())
    {
        auto selectorValue = selector->asString();
        if(selectorValue == "initialize")
            getInitializeMethod();
        else if(selectorValue == "initializeCopyingFrom:")
            getInitializeCopyingFromMethod();
        else if(selectorValue == "initializeMovingFrom:")
            getInitializeMovingFromMethod();
        else if(selectorValue == ":=")
        {
            getAssignCopyingFromMethod();
            getAssignMovingFromMethod();
        }
    }

    return lookupExistentLocalSelector(selector);
}

AnyValuePtr Type::lookupExistentLocalSelector(const AnyValuePtr &selector)
{
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
    auto it = bindings.find(symbol);
    if(it != bindings.end())
        return it->second.second;

    if(accessingScope->isProgramEntityScope() && symbol->isLiteralSymbol())
    {
        auto symbolValue = symbol->asString();
        if(symbolValue == "SelfType" || symbolValue == "self")
            return selfFromThis();
        else if(symbolValue == "meta")
            return getMetaType();
        else if(symbolValue == "InstanceType")
            return getInstanceType();
    }

    return nullptr;
}

AnyValuePtr Type::lookupLocalSymbolRecursivelyFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    auto result = lookupLocalSymbolFromScope(symbol, accessingScope);
    if(result)
        return result;

    if(supertype)
        return supertype->lookupLocalSymbolFromScope(symbol, accessingScope);
    return nullptr;
}

ASTNodePtr Type::analyzeCallNode(const ASTCallNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    sysmelAssert(node->function && node->function->analyzedType);

    // In case the fuction is an error, analyze the arguments to
    // further discover more error, and then return the receiver to propagate an error node.
    if(isCompilationErrorValueType())
    {
        sysmelAssert(node->function->analyzedType->isCompilationErrorValueType());
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
    sysmelAssert(node->receiver && node->receiver->analyzedType);
    sysmelAssert(node->selector && node->selector->analyzedType);

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
    sysmelAssert(node->receiver && node->receiver->analyzedType);
    sysmelAssert(node->selector && node->selector->analyzedType);

    // In case the receiver is an error, analyze the arguments to
    // further discover more error, and then return the receiver to propagate an error node.
    if(isCompilationErrorValueType())
    {
        sysmelAssert(node->receiver->analyzedType->isCompilationErrorValueType());
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
    return hasTrivialLifetimeMethod("initialize", {}, getVoidType());
}

AnyValuePtr Type::getInitializeMethod()
{
    return lookupValidLifetimeMethod("initialize", {}, getVoidType());
}

bool Type::hasTrivialInitializationCopyingFrom()
{
    return hasTrivialLifetimeMethod("initializeCopyingFrom:", {asConstReceiverType()}, getVoidType());
}

AnyValuePtr Type::getInitializeCopyingFromMethod()
{
    return lookupValidLifetimeMethod("initializeCopyingFrom:", {asConstReceiverType()}, getVoidType());
}

bool Type::hasTrivialInitializationMovingFrom()
{
    return hasTrivialLifetimeMethod("initializeMovingFrom:", {tempRef()}, getVoidType());
}

AnyValuePtr Type::getInitializeMovingFromMethod()
{
    return lookupValidLifetimeMethod("initializeMovingFrom:", {tempRef()}, getVoidType());
}

bool Type::hasTrivialFinalization()
{
    return hasTrivialLifetimeMethod("finalize", {}, getVoidType());
}

AnyValuePtr Type::getFinalizeMethod()
{
    return lookupValidLifetimeMethod("finalize", {}, getVoidType());
}

bool Type::hasTrivialAssignCopyingFrom()
{
    return hasTrivialLifetimeMethod(":=", {asConstReceiverType()}, asConstReceiverType());
}

AnyValuePtr Type::getAssignCopyingFromMethod()
{
    return lookupValidLifetimeMethod(":=", {asConstReceiverType()}, asConstReceiverType());
}

bool Type::hasTrivialAssignMovingFrom()
{
    return hasTrivialLifetimeMethod(":=", {tempRef()}, asConstReceiverType());
}

AnyValuePtr Type::getAssignMovingFromMethod()
{
    return lookupValidLifetimeMethod(":=", {tempRef()}, asConstReceiverType());
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
    auto alignment = getMemoryAlignment();
    auto memorySize = getMemorySize();
    if(alignment == 0)
        return 0;
    return alignedTo(memorySize, alignment);
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
    sysmelAssert(!field->getParentProgramEntity());
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

void Type::recordPotentialSpecialMethod(const AnyValuePtr &method)
{
    if(!validAnyValue(method)->isSpecificMethod())
        return;
    
    auto specificMethod = method.staticAs<SpecificMethod> ();
    if(specificMethod->isConstructor())
        getInstanceType()->addConstructor(method);

    if(specificMethod->isConversion())
        addConversion(method);
}

void Type::addMacroMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    recordPotentialSpecialMethod(method);
    if(!macroMethodDictionary)
        macroMethodDictionary = basicMakeObject<MethodDictionary> ();
    macroMethodDictionary->addMethodWithSelector(method, selector);
}

void Type::replaceMacroMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    recordPotentialSpecialMethod(method);
    if(!macroMethodDictionary)
        macroMethodDictionary = basicMakeObject<MethodDictionary> ();
    macroMethodDictionary->replaceMethodWithSelector(method, selector);
}

void Type::addMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    recordPotentialSpecialMethod(method);
    if(!methodDictionary)
        methodDictionary = basicMakeObject<MethodDictionary> ();
    methodDictionary->addMethodWithSelector(method, selector);
}

void Type::addMacroFallbackMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector)
{
    recordPotentialSpecialMethod(method);
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

TypeConversionCost Type::rankToMatchType(const TypePtr &type)
{
    uint32_t upCastLength = 0;
    for(auto currentType = type; currentType; currentType = currentType->getSupertype())
    {
        if(currentType.get() == this)
        {
            if(upCastLength == 0)
                return TypeConversionCost{DirectTypeConversionCost::Identity};
            sysmelAssert(upCastLength > 0);
            return TypeConversionCost{DirectTypeConversionCost::Upcast, upCastLength - 1};
        }

        ++upCastLength;
    }

    return TypeConversionCost{};
}

TypeConversionCost Type::rankToMatchValue(const AnyValuePtr &value)
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
    sysmelAssert(!oldParent || oldParent == selfFromThis());
    if(oldParent)
        return;

    children.push_back(newChild);
    newChild->setParentProgramEntity(selfFromThis());
}

void Type::bindSymbolWithVisibility(const AnyValuePtr &symbol, ProgramEntityVisibility visibility, const ProgramEntityPtr &binding)
{
    sysmelAssert(symbol && !symbol->isAnonymousNameSymbol());
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
    addTypeConversionRule(ValueAsConstRefTypeConversionRule::uniqueInstance());
    addTypeConversionRule(ValueInVariantTypeConversionRule::uniqueInstance());
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

TypeConversionRulePtrList Type::getAllImplicitTypeConversionRules()
{
    evaluateAllPendingBodyBlockCodeFragments();
    TypeConversionRulePtrList result = implicitTypeConversionRules;

    // Find a conversion method here.
    for(auto &conversion : conversions)
    {
        if(conversion->isExplicit())
            continue;

        const auto &conversionType = conversion->getFunctionalType();
        auto sourceType = conversionType->getReceiverType();
        auto targetType = conversionType->getResultType();
        result.push_back(ConversionMethodTypeConversionRule::makeFor(sourceType, targetType, conversion));
    }

    // Find the constructor methods here.
    for(auto &targetConstructor : constructors)
    {
        const auto &constructorType = targetConstructor->getFunctionalType();
        if(constructorType->getArgumentCount() != 1)
            continue;

        if(targetConstructor->isExplicit())
            continue;

        auto sourceType = constructorType->getArgument(0);
        auto targetType = constructorType->getResultType();
        result.push_back(ConstructorMethodTypeConversionRule::makeFor(sourceType, targetType, targetConstructor));
    }

    return result;
}

TypeConversionRulePtrList Type::getAllExplicitTypeConversionRules()
{
    evaluateAllPendingBodyBlockCodeFragments();
    TypeConversionRulePtrList result = explicitTypeConversionRules;

    // Find a conversion method here.
    for(auto &conversion : conversions)
    {
        const auto &conversionType = conversion->getFunctionalType();
        auto sourceType = conversionType->getReceiverType();
        auto targetType = conversionType->getResultType();
        result.push_back(ConversionMethodTypeConversionRule::makeFor(sourceType, targetType, conversion));
    }

    // Find the constructor methods here.
    for(auto &targetConstructor : constructors)
    {
        const auto &constructorType = targetConstructor->getFunctionalType();
        if(constructorType->getArgumentCount() != 1)
            continue;

        auto sourceType = constructorType->getArgument(0);
        auto targetType = constructorType->getResultType();
        result.push_back(ConstructorMethodTypeConversionRule::makeFor(sourceType, targetType, targetConstructor));
    }

    return result;
}

template<typename FT>
static TypeConversionRulePtr findTypeConversionRule(const ASTNodePtr &node, const TypePtr &sourceType, const TypePtr &targetType, FT &&ruleExtractionFunction)
{
    struct TypeConversionRuleWithCost
    {
        TypeConversionRulePtr rule;
        TypeConversionCost cost;
        TypePtr intermediateTargetType;

        bool operator<(const TypeConversionRuleWithCost &o) const
        {
            return o.cost < cost;
        }
    };

    std::priority_queue<TypeConversionRuleWithCost> pendingRulesToEvaluate;
    std::unordered_set<TypePtr> visitedTypeForTransposedRules;
    std::unordered_map<TypePtr, TypeConversionRulePtrList> transposedRules;

    auto&& findTransposedRules = [&](const TypePtr &type, auto&& recursion)
    {
        if(visitedTypeForTransposedRules.find(type) != visitedTypeForTransposedRules.end())
            return;
        visitedTypeForTransposedRules.insert(type);

        for(auto &rule : ruleExtractionFunction(type))
        {
            auto canonicalSourceType = rule->getCanonicalSourceTypeFor(type);
            if(!canonicalSourceType || canonicalSourceType == type)
                continue;

            // Is this a valid transposed rule?
            if(rule->canBeUsedToConvertNodeFromTo(node, canonicalSourceType, type))
            {
                transposedRules[canonicalSourceType].push_back(rule);
                recursion(canonicalSourceType, recursion);
            }
        }
    };
    findTransposedRules(targetType, findTransposedRules);

    std::unordered_set<TypePtr> visitedTypes;

    auto&& expandRulesWithType = [&](const TypeConversionRulePtr &incomingRule, const TypeConversionCost &incomingCost, const TypePtr &intermediateSourceType)
    {
        if(visitedTypes.find(intermediateSourceType) != visitedTypes.end())
            return;

        visitedTypes.insert(intermediateSourceType);

        auto && addRule = [&](const TypeConversionRulePtr &rule, const TypePtr &ruleTargetType) {
            auto cost = rule->getConversionCost(node, intermediateSourceType, ruleTargetType);
            sysmelAssert(!cost.isInvalid());

            auto newRule = rule;
            auto newRuleCost = cost;
            if(incomingRule)
            {
                auto chainedRule = basicMakeObject<ChainedTypeConversionRule> ();
                chainedRule->firstConversionRule = incomingRule;
                chainedRule->intermediateType = intermediateSourceType;
                chainedRule->secondConversionRule = rule;

                newRule = chainedRule;
                newRuleCost = TypeConversionCost{
                    uint8_t(incomingCost.directCost) > uint8_t(cost.directCost) ? incomingCost.directCost : cost.directCost,
                    incomingCost.chainLength + cost.chainLength + 1
                };
                sysmelAssert(!newRuleCost.isInvalid());
            }

            pendingRulesToEvaluate.push(TypeConversionRuleWithCost{newRule, newRuleCost, ruleTargetType});
        };

        auto && expandRule = [&](const TypeConversionRulePtr &rule) {
            if(rule->canBeUsedToConvertNodeFromTo(node, intermediateSourceType, targetType))
            {
                addRule(rule, targetType);
                return;
            }

            auto canonicalTargetType = rule->getCanonicalTargetTypeFor(intermediateSourceType);
            if(!canonicalTargetType || canonicalTargetType == intermediateSourceType)
                return;
            
            if(rule->canBeUsedToConvertNodeFromTo(node, intermediateSourceType, canonicalTargetType))
                addRule(rule, canonicalTargetType);
        };

        // Expand the rules defined in the type.
        for(auto &rule : ruleExtractionFunction(intermediateSourceType))
            expandRule(rule);

        // Expand the transposed rules
        {
            auto it = transposedRules.find(intermediateSourceType);
            if(it != transposedRules.end())
            {
                for(auto &rule : it->second)
                    expandRule(rule);
            }
        }
    };

    expandRulesWithType(nullptr, TypeConversionCost{}, sourceType);

    while(!pendingRulesToEvaluate.empty())
    {
        auto [rule, cost, intermediateTargetType] = pendingRulesToEvaluate.top();
        pendingRulesToEvaluate.pop();

        // Is this the rule that we are looking for?
        if(intermediateTargetType == targetType)
            return rule;

        // Keep expanding the target type.
        expandRulesWithType(rule, cost, intermediateTargetType);
    }

    return nullptr;
}

TypeConversionRulePtr Type::findImplicitTypeConversionRuleForInto(const ASTNodePtr &node, const TypePtr &targetType, bool isReceiverType)
{
    if(isReceiverType &&
        ValueAsReceiverReferenceTypeConversionRule::uniqueInstance()->canBeUsedToConvertNodeFromTo(node, selfFromThis(), targetType) )
    {
        return ValueAsReceiverReferenceTypeConversionRule::uniqueInstance();
    }

    auto rule = RuntimeContext::getActive()->findCachedTypeConversionRuleFor(node, selfFromThis(), targetType, false);
    if(rule)
    {
        if(!rule->canBeUsedToConvertNodeFromTo(node, selfFromThis(), targetType))
            return nullptr;

        return rule;
    }

    rule = findTypeConversionRule(node, selfFromThis(), targetType, [](const TypePtr &type){
        return type->getAllImplicitTypeConversionRules();
    });

    RuntimeContext::getActive()->setCachedTypeConversionRuleFor(node, selfFromThis(), targetType, false, rule);
    return rule;
}

TypeConversionRulePtr Type::findExplicitTypeConversionRuleForInto(const ASTNodePtr &node, const TypePtr &targetType)
{
    auto rule = RuntimeContext::getActive()->findCachedTypeConversionRuleFor(node, selfFromThis(), targetType, true);
    if(rule)
    {
        if(!rule->canBeUsedToConvertNodeFromTo(node, selfFromThis(), targetType))
            return nullptr;

        return rule;
    }

    rule = findTypeConversionRule(node, selfFromThis(), targetType, [](const TypePtr &type){
        return type->getAllExplicitTypeConversionRules();
    });

    RuntimeContext::getActive()->setCachedTypeConversionRuleFor(node, selfFromThis(), targetType, true, rule);
    return rule;
}

bool Type::canBeReinterpretedAsType(const TypePtr &otherType)
{

    auto selfUndecoratedType = selfFromThis()->asUndecoratedType();
    auto otherUndecoratedType = otherType->asUndecoratedType();

    return
        selfUndecoratedType->getMemorySize() == otherUndecoratedType->getMemorySize() &&
        selfUndecoratedType->isReferenceType() == otherUndecoratedType->isReferenceType() &&
        selfUndecoratedType->isTemporaryReferenceType() == otherUndecoratedType->isTemporaryReferenceType() &&
        hasTrivialAssignCopyingFrom() && otherUndecoratedType->hasTrivialAssignCopyingFrom();
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

    sysmelAssert("TODO: expandCopyConstruction" && false);
}

ASTNodePtr Type::expandMoveConstruction(const MacroInvocationContextPtr &context, const ASTNodePtr &valueNode)
{
    (void)context;
    if(isImmutableType())
        return valueNode;

    sysmelAssert("TODO: expandMoveConstruction" && false);
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
        PatternMatchingRank bestRank;

        for(auto &constructor : constructors)
        {
            auto result = constructor->matchPatternForAnalyzingMessageSendNode(messageSendNode, semanticAnalyzer);
            if(!result.matchingMethod)
                continue;

            if(bestRank.empty() || result.matchingRank < bestRank)
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

    return analyzeFallbackValueConstructionWithArguments(node, arguments, semanticAnalyzer);
}

ASTNodePtr Type::analyzeFallbackValueConstructionWithArguments(const ASTNodePtr &node, const ASTNodePtrList &, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
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

ReferenceTypePtr Type::refForMemberOfReceiverWithType(const TypePtr &receiverType)
{
    AnyValuePtr receiverAddressSpace;
    TypeDecorationFlags receiverDecorations = receiverType->getDecorationFlags();
    if(receiverType->isPointerLikeType())
        receiverDecorations = receiverType->getBaseType()->getDecorationFlags();

    return selfFromThis()->withDecorations(receiverDecorations)->refFor(receiverAddressSpace);
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
    newElementTypes.push_back(nextType);
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

TypePtr Type::withParams()
{
    return withDecorations(TypeDecorationFlags::Params);
}

TypePtr Type::withDecorations(TypeDecorationFlags decorations)
{
    return DecoratedType::make(selfFromThis(), decorations);
}

TypePtr Type::asUndecoratedType()
{
    return selfFromThis();
}

TypePtr Type::getBaseType()
{
    SysmelSelfSubclassResponsibility();
}

TypePtr Type::asDecayedType()
{
    return selfFromThis();
}

TypePtr Type::asCanonicalArgumentType()
{
    return asUndecoratedType();
}

TypePtr Type::asCanonicalResultType()
{
    return asUndecoratedType();
}

bool Type::isConstOrConstReferenceType() const
{
    return isConstDecoratedType();
}

TypePtr Type::asConstOrConstReferenceType()
{
    return withConst();
}

TypeDecorationFlags Type::getDecorationFlags() const
{
    return TypeDecorationFlags::None;
}

SSAValuePtr Type::asSSAValueRequiredInPosition(const ASTSourcePositionPtr &)
{
    if(!ssaTypeProgramEntity)
    {
        ssaTypeProgramEntity = basicMakeObject<SSATypeProgramEntity> ();
        ssaTypeProgramEntity->setName(getValidName());
        ssaTypeProgramEntity->setValue(selfFromThis());
        ssaTypeProgramEntity->setExternalLanguageMode(externalLanguageMode);
        ssaTypeProgramEntity->setVisibility(visibility);
        ssaTypeProgramEntity->setDllLinkageMode(dllLinkageMode);
        auto parentProgramEntity = getParentProgramEntity();
        if(parentProgramEntity)
        {
            auto parentSSAValue = parentProgramEntity->asProgramEntitySSAValue();
            if(parentSSAValue->isSSAProgramEntity())
            {
                sysmelAssert(parentSSAValue->isSSAProgramEntity());
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

void Type::ensureImplicitLifeTimeMethodsWithSelectorAreCreated(const std::string &selector)
{
    (void)selector;
}

AnyValuePtr Type::lookupValidLifetimeMethod(const std::string &selector, const TypePtrList &argumentTypes, const TypePtr &resultType)
{
    evaluateAllPendingBodyBlockCodeFragments();
    ensureImplicitLifeTimeMethodsWithSelectorAreCreated(selector);
    return lookupExistentLocalMethodWithSignature(internSymbol(selector), argumentTypes, resultType);
}

AnyValuePtr Type::lookupExistentLocalMethodWithSignature(const AnyValuePtr &selector, const TypePtrList &argumentTypes, const TypePtr &resultType, MethodFlags signatureMethodFlags)
{
    (void)signatureMethodFlags;
    if(!methodDictionary)
        return nullptr;

    auto method = methodDictionary->lookupSelector(selector);
    if(!method)
        return nullptr;

    return method->asMethodMatchingSignature(asReceiverType(), argumentTypes, resultType);
}

bool Type::hasTrivialLifetimeMethod(const std::string &selector, const TypePtrList &argumentTypes, const TypePtr &resultType)
{
    auto method = lookupExistentLocalMethodWithSignature(internSymbol(selector), argumentTypes, resultType);
    if(!method || method->isUndefined())
        return true;

    if(!method->isSpecificMethod())
        return false;

    return method.staticAs<SpecificMethod> ()->isTrivial();
}

const ASTSourcePositionPtr &Type::getSourceDefinitionPosition() const
{
    return definitionPosition;
}

void Type::setSourceDefinitionPosition(const ASTSourcePositionPtr &position)
{
    definitionPosition = position;
}

void Type::ensureVirtualTableLayoutComputation()
{
}

bool Type::matchesExpectedValueTypeInPattern(const TypePtr &typeToMatch)
{
    return asDecayedType() == typeToMatch->asDecayedType();
}

} // End of namespace Environment
} // End of namespace Sysmel