#include "sysmel/BootstrapEnvironment/AnyValue.hpp"
#include "sysmel/BootstrapEnvironment/ASTNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTCallNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTIdentifierReferenceNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTMessageSendNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTLiteralValueNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTSemanticAnalyzer.hpp"
#include "sysmel/BootstrapEnvironment/ASTBuilder.hpp"
#include "sysmel/BootstrapEnvironment/ASTExplicitCastNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTImplicitCastNode.hpp"
#include "sysmel/BootstrapEnvironment/ASTReinterpretCastNode.hpp"
#include "sysmel/BootstrapEnvironment/SSAConstantLiteralValue.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/SubclassResponsibility.hpp"
#include "sysmel/BootstrapEnvironment/CannotUnwrap.hpp"
#include "sysmel/BootstrapEnvironment/CannotEvaluateMessage.hpp"
#include "sysmel/BootstrapEnvironment/MessageNotUnderstood.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapMethod.hpp"
#include "sysmel/BootstrapEnvironment/MacroInvocationContext.hpp"
#include "sysmel/BootstrapEnvironment/ValueBox.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"
#include <algorithm>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<AnyValue> anyValueTypeRegistration;

TypePtr Type::getAnyValueType()
{
    return AnyValue::__staticType__();
}

MethodCategories AnyValue::__instanceMethods__()
{
    return MethodCategories{
        {"comparisons", {
            makeIntrinsicMethodBinding<bool (AnyValuePtr, AnyValuePtr)> ("object.identity.equals", "==", +[](const AnyValuePtr &a, const AnyValuePtr &b){
                return a == b;
            }, MethodFlags::Pure),
            makeIntrinsicMethodBinding<bool (AnyValuePtr, AnyValuePtr)> ("object.identity.not-equals", "~~", +[](const AnyValuePtr &a, const AnyValuePtr &b){
                return a != b;
            }, MethodFlags::Pure),
        }},

        {"accessing", {
            makeIntrinsicMethodBinding("object.get-type", "__type__", &AnyValue::getType, MethodFlags::Pure),
            makeIntrinsicMethodBinding<AnyValuePtr (AnyValuePtr)> ("object.yourself", "yourself", [](const AnyValuePtr &self) {
                return self;
            }, MethodFlags::Pure),
        }},

        {"initialization", {
            makeIntrinsicMethodBinding("object.initialize", "initialize", &AnyValue::initialize),
        }},

        {"printing", {
            makeMethodBinding("printString", &AnyValue::printString),
            makeMethodBinding("asString", &AnyValue::asString),
        }}
    };
}

MethodCategories AnyValue::__typeMethods__()
{
    return MethodCategories{};
}

AnyValuePtrList AnyValue::__constructors__()
{
    return AnyValuePtrList{};
}

AnyValuePtrList AnyValue::__conversions__()
{
    return AnyValuePtrList{};
}

MethodCategories AnyValue::__instanceMacroMethods__()
{
    return MethodCategories{
        {"accessing", {
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr)> ("yourself", [](const MacroInvocationContextPtr &macroContext) {
                return macroContext->receiverNode;
            }, MethodFlags::Macro),
        }},

        {"casting", {
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr)> ("castTo:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &targetType) {
                return macroContext->astBuilder->explicitCastTo(macroContext->receiverNode, targetType);
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr)> ("explicitCastTo:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &targetType) {
                return macroContext->astBuilder->explicitCastTo(macroContext->receiverNode, targetType);
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr)> ("implicitCastTo:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &targetType) {
                return macroContext->astBuilder->implicitCastTo(macroContext->receiverNode, targetType);
            }, MethodFlags::Macro),
            makeMethodBinding<ASTNodePtr (MacroInvocationContextPtr, ASTNodePtr)> ("reinterpretCastTo:", [](const MacroInvocationContextPtr &macroContext, const ASTNodePtr &targetType) {
                return macroContext->astBuilder->reinterpretCastTo(macroContext->receiverNode, targetType);
            }, MethodFlags::Macro),
        }}
    };
}

MethodCategories AnyValue::__typeMacroMethods__()
{
    return MethodCategories{};
}

void AnyValue::__addTypeConversionRules__(const TypePtr &type)
{
    (void)type;
}

AnyValuePtr AnyValue::__basicNewValue__()
{
    return basicMakeObject<AnyValue> ();
}

bool AnyValue::__canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value)
{
    (void)value;
    return false;
}

AnyValuePtr AnyValue::__instantiateWithLiteralValue__(const AnyValuePtr &value)
{
    (void)value;
    return nullptr;
}

TypePtr AnyValue::__asInferredTypeForWithModeInEnvironment__(const TypePtr &selfType, const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment)
{
    (void)node;
    (void)mode;
    (void)isMutable;
    (void)concreteLiterals;
    (void)environment;
    return selfType;
}

AnyValue::~AnyValue()
{
}

void AnyValue::initialize()
{
}

bool AnyValue::isKindOf(const TypePtr &type) const
{
    return getType()->isSubtypeOf(type);
}

bool AnyValue::isCompilerObject() const
{
    return false;
}

bool AnyValue::isASTNode() const
{
    return false;
}

bool AnyValue::isASTBuilder() const
{
    return false;
}

bool AnyValue::isASTArgumentDefinitionNode() const
{
    return false;
}

bool AnyValue::isASTIdentifierReferenceNode() const
{
    return false;
}

bool AnyValue::isASTCallNode() const
{
    return false;
}

bool AnyValue::isASTMakeTupleNode() const
{
    return false;
}

bool AnyValue::isASTMakeLiteralArrayNode() const
{
    return false;
}

bool AnyValue::isASTMakeDictionaryNode() const
{
    return false;
}

bool AnyValue::isASTMakeAssociationNode() const
{
    return false;
}

bool AnyValue::isASTMessageChainNode() const
{
    return false;
}

bool AnyValue::isASTMessageChainMessageNode() const
{
    return false;
}

bool AnyValue::isASTMessageSendNode() const
{
    return false;
}

bool AnyValue::isASTLiteralValueNode() const
{
    return false;
}

bool AnyValue::isASTSequenceNode() const
{
    return false;
}

bool AnyValue::isASTCleanUpScopeNode() const
{
    return false;
}

bool AnyValue::isASTLexicalScopeNode() const
{
    return false;
}

bool AnyValue::isASTClosureNode() const
{
    return false;
}

bool AnyValue::isASTErrorNode() const
{
    return false;
}

bool AnyValue::isASTParseErrorNode() const
{
    return false;
}

bool AnyValue::isASTSemanticErrorNode() const
{
    return false;
}

bool AnyValue::isASTCompileTimeEvaluationErrorNode() const
{
    return false;
}

bool AnyValue::isASTPragmaNode() const
{
    return false;
}

bool AnyValue::isASTQuasiQuoteNode() const
{
    return false;
}

bool AnyValue::isASTQuasiUnquoteNode() const
{
    return false;
}

bool AnyValue::isASTQuoteNode() const
{
    return false;
}

bool AnyValue::isASTSourceCode() const
{
    return false;
}

bool AnyValue::isASTSourcePosition() const
{
    return false;
}

bool AnyValue::isASTEmptySourcePosition() const
{
    return false;
}

bool AnyValue::isASTSourceCodePosition() const
{
    return false;
}

bool AnyValue::isASTSpliceNode() const
{
    return false;
}

bool AnyValue::isASTProgramEntityNode() const
{
    return false;
}

bool AnyValue::isASTVariableNode() const
{
    return false;
}

bool AnyValue::isASTLocalVariableNode() const
{
    return false;
}

bool AnyValue::isASTGlobalVariableNode() const
{
    return false;
}

bool AnyValue::isASTFieldVariableNode() const
{
    return false;
}

bool AnyValue::isASTCompileTimeConstantNode() const
{
    return false;
}

bool AnyValue::isASTLocalImmutableAccessNode() const
{
    return false;
}

bool AnyValue::isASTVariableAccessNode() const
{
    return false;
}

bool AnyValue::isASTFunctionalNode() const
{
    return false;
}

bool AnyValue::isASTFunctionNode() const
{
    return false;
}

bool AnyValue::isASTMethodNode() const
{
    return false;
}

bool AnyValue::isASTTemplateNode() const
{
    return false;
}

bool AnyValue::isASTNamespaceNode() const
{
    return false;
}

bool AnyValue::isASTTypeNode() const
{
    return false;
}

bool AnyValue::isASTStructNode() const
{
    return false;
}

bool AnyValue::isASTUnionNode() const
{
    return false;
}

bool AnyValue::isASTClassNode() const
{
    return false;
}

bool AnyValue::isASTEnumNode() const
{
    return false;
}

bool AnyValue::isASTProgramEntityExtensionNode() const
{
    return false;
}

bool AnyValue::isASTCastNode() const
{
    return false;
}

bool AnyValue::isASTImplicitCastNode() const
{
    return false;
}

bool AnyValue::isASTExplicitCastNode() const
{
    return false;
}

bool AnyValue::isASTReinterpretCastNode() const
{
    return false;
}

bool AnyValue::isASTTypeConversionNode() const
{
    return false;
}

bool AnyValue::isASTValueAsVoidTypeConversionNode() const
{
    return false;
}

bool AnyValue::isASTUpcastTypeConversionNode() const
{
    return false;
}

bool AnyValue::isASTDowncastTypeConversionNode() const
{
    return false;
}

bool AnyValue::isASTIfNode() const
{
    return false;
}

bool AnyValue::isASTWhileNode() const
{
    return false;
}

bool AnyValue::isASTDoWhileNode() const
{
    return false;
}

bool AnyValue::isASTReturnNode() const
{
    return false;
}

bool AnyValue::isASTBreakNode() const
{
    return false;
}

bool AnyValue::isASTContinueNode() const
{
    return false;
}

bool AnyValue::isIdentifierLookupScope() const
{
    return false;
}

bool AnyValue::isCleanUpScope() const
{
    return false;
}

bool AnyValue::isCompileTimeCleanUpScope() const
{
    return false;
}

bool AnyValue::isProgramEntity() const
{
    return false;
}

bool AnyValue::isType() const
{
    return false;
}

bool AnyValue::isMetaType() const
{
    return false;
}

bool AnyValue::isSimpleType() const
{
    return false;
}

bool AnyValue::isBootstrapType() const
{
    return false;
}

bool AnyValue::isMacroInvocationContext() const
{
    return false;
}

bool AnyValue::isMacroMethod() const
{
    return false;
}

bool AnyValue::isMethod() const
{
    return false;
}

bool AnyValue::isMethodDictionary() const
{
    return false;
}

bool AnyValue::isSpecificMethod() const
{
    return false;
}

bool AnyValue::isCompileTimeEvaluableMethod() const
{
    return false;
}

bool AnyValue::isCompiledMethod() const
{
    return false;
}

bool AnyValue::isBootstrapMethod() const
{
    return false;
}

bool AnyValue::isPatternMatchingMethod() const
{
    return false;
}

bool AnyValue::isTemplateMethod() const
{
    return false;
}

bool AnyValue::isNamespace() const
{
    return false;
}

bool AnyValue::isMetaBuilder() const
{
    return false;
}

bool AnyValue::isVariable() const
{
    return false;
}

bool AnyValue::isArgumentVariable() const
{
    return false;
}

bool AnyValue::isLocalVariable() const
{
    return false;
}

bool AnyValue::isFieldVariable() const
{
    return false;
}

bool AnyValue::isGlobalVariable() const
{
    return false;
}

bool AnyValue::isPragma() const
{
    return false;
}

bool AnyValue::isLiteralValue() const
{
    return false;
}

bool AnyValue::isLiteralArray() const
{
    return false;
}

bool AnyValue::isLiteralAssociation() const
{
    return false;
}

bool AnyValue::isLiteralDictionary() const
{
    return false;
}

bool AnyValue::isLiteralNumber() const
{
    return false;
}

bool AnyValue::isLiteralInteger() const
{
    return false;
}

bool AnyValue::isLiteralPositiveInteger() const
{
    return false;
}

bool AnyValue::isLiteralNegativeInteger() const
{
    return false;
}

bool AnyValue::isLiteralCharacter() const
{
    return false;
}

bool AnyValue::isLiteralFraction() const
{
    return false;
}

bool AnyValue::isLiteralFloat() const
{
    return false;
}

bool AnyValue::isLiteralString() const
{
    return false;
}

bool AnyValue::isLiteralSymbol() const
{
    return false;
}

bool AnyValue::isLiteralIdentifierSymbol() const
{
    return false;
}

bool AnyValue::isLiteralBoolean() const
{
    return false;
}

bool AnyValue::isLiteralBooleanTrue() const
{
    return false;
}

bool AnyValue::isLiteralBooleanFalse() const
{
    return false;
}

bool AnyValue::isVoid() const
{
    return false;
}

bool AnyValue::isUndefined() const
{
    return false;
}

bool AnyValue::isCompilationErrorValue() const
{
    return false;
}

bool AnyValue::isPureCompileTimeLiteralValue() const
{
    return false;
}

bool AnyValue::isAnonymousNameSymbol() const
{
    return false;
}

bool AnyValue::isValueBox() const
{
    return false;
}

bool AnyValue::isFunctionalType() const
{
    return false;
}

bool AnyValue::isFunctionalTypeValue() const
{
    return false;
}

bool AnyValue::isFunctionType() const
{
    return false;
}

bool AnyValue::isFunctionTypeValue() const
{
    return false;
}

bool AnyValue::isMethodType() const
{
    return false;
}

bool AnyValue::isMethodTypeValue() const
{
    return false;
}

bool AnyValue::isClosureType() const
{
    return false;
}

bool AnyValue::isClosureTypeValue() const
{
    return false;
}

bool AnyValue::isPrimitiveTypeValue() const
{
    return false;
}

bool AnyValue::isPrimitiveTensorTypeValue() const
{
    return false;
}

bool AnyValue::isPrimitiveScalarTypeValue() const
{
    return false;
}

bool AnyValue::isPrimitiveBooleanTypeValue() const
{
    return false;
}

bool AnyValue::isPrimitiveNumberTypeValue() const
{
    return false;
}

bool AnyValue::isPrimitiveIntegerTypeValue() const
{
    return false;
}

bool AnyValue::isPrimitiveCharacterTypeValue() const
{
    return false;
}

bool AnyValue::isPrimitiveFloatTypeValue() const
{
    return false;
}

bool AnyValue::isAggregateType() const
{
    return false;
}

bool AnyValue::isAggregateTypeValue() const
{
    return false;
}

bool AnyValue::isAggregateTypeWithFields() const
{
    return false;
}

bool AnyValue::isAggregateTypeWithFieldsValue() const
{
    return false;
}

bool AnyValue::isStructureType() const
{
    return false;
}

bool AnyValue::isStructureTypeValue() const
{
    return false;
}

bool AnyValue::isClassType() const
{
    return false;
}

bool AnyValue::isClassTypeValue() const
{
    return false;
}

bool AnyValue::isUnionType() const
{
    return false;
}

bool AnyValue::isUnionTypeValue() const
{
    return false;
}

bool AnyValue::isTupleType() const
{
    return false;
}

bool AnyValue::isTupleTypeValue() const
{
    return false;
}

bool AnyValue::isVariantType() const
{
    return false;
}

bool AnyValue::isVariantTypeValue() const
{
    return false;
}

bool AnyValue::isDerivedType() const
{
    return false;
}

bool AnyValue::isDecoratedType() const
{
    return false;
}

bool AnyValue::isDecoratedTypeValue() const
{
    return false;
}

bool AnyValue::isConstDecoratedType() const
{
    return false;
}

bool AnyValue::isVolatileDecoratedType() const
{
    return false;
}

bool AnyValue::isRestrictDecoratedType() const
{
    return false;
}

bool AnyValue::isDerivedTypeValue() const
{
    return false;
}

bool AnyValue::isEnumType() const
{
    return false;
}

bool AnyValue::isEnumTypeValue() const
{
    return false;
}

bool AnyValue::isPointerLikeType() const
{
    return false;
}

bool AnyValue::isPointerLikeTypeValue() const
{
    return false;
}

bool AnyValue::isReferenceLikeType() const
{
    return false;
}

bool AnyValue::isReferenceLikeTypeValue() const
{
    return false;
}

bool AnyValue::isPointerType() const
{
    return false;
}

bool AnyValue::isPointerTypeValue() const
{
    return false;
}

bool AnyValue::isReferenceType() const
{
    return false;
}

bool AnyValue::isReferenceTypeValue() const
{
    return false;
}

bool AnyValue::isTemporaryReferenceType() const
{
    return false;
}

bool AnyValue::isTemporaryReferenceTypeValue() const
{
    return false;
}

bool AnyValue::isArrayType() const
{
    return false;
}

bool AnyValue::isArrayTypeValue() const
{
    return false;
}

bool AnyValue::isCompileTimeConstant() const
{
    return false;
}

bool AnyValue::isEnumConstant() const
{
    return false;
}

bool AnyValue::isTemplate() const
{
    return false;
}

bool AnyValue::isTemplateInstance() const
{
    return false;
}

bool AnyValue::isSSAValue() const
{
    return false;
}

bool AnyValue::isSSAConstantValue() const
{
    return false;
}

bool AnyValue::isSSAConstantLiteralValue() const
{
    return false;
}

bool AnyValue::isSSAGlobalValue() const
{
    return false;
}

bool AnyValue::isSSAGlobalVariable() const
{
    return false;
}

bool AnyValue::isSSAFunction() const
{
    return false;
}

bool AnyValue::isSSACodeRegion() const
{
    return false;
}

bool AnyValue::isSSABasicBlock() const
{
    return false;
}

bool AnyValue::isSSACodeRegionLocalValue() const
{
    return false;
}

bool AnyValue::isSSACodeRegionArgument() const
{
    return false;
}

bool AnyValue::isSSAInstruction() const
{
    return false;
}

bool AnyValue::isSSACallInstruction() const
{
    return false;
}

bool AnyValue::isSSASendMessageInstruction() const
{
    return false;
}

bool AnyValue::isSSAJumpInstruction() const
{
    return false;
}

bool AnyValue::isSSAConditionalJumpInstruction() const
{
    return false;
}

bool AnyValue::isSSAIfInstruction() const
{
    return false;
}

bool AnyValue::isSSAWhileInstruction() const
{
    return false;
}

bool AnyValue::isSSADoWhileInstruction() const
{
    return false;
}

bool AnyValue::isSSADoWithCleanupInstruction() const
{
    return false;
}

bool AnyValue::isSSAMakeClosureInstruction() const
{
    return false;
}

bool AnyValue::isSSAReturnFromRegionInstruction() const
{
    return false;
}

bool AnyValue::isSSAReturnFromFunctionInstruction() const
{
    return false;
}

bool AnyValue::isSSAUnreachableInstruction() const
{
    return false;
}

std::string AnyValue::asString() const
{
    return printString();
}

std::string AnyValue::printString() const
{
    return "a " + getType()->printString();
}

std::string AnyValue::fullPrintString() const
{
    return printString();
}

SExpression AnyValue::asSExpression() const
{
    SysmelSelfSubclassResponsibility();
}

SExpression AnyValue::asFullDefinitionSExpression() const
{
    return asSExpression();
}

ASTNodePtr AnyValue::asASTNodeRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition)
{
    auto result = basicMakeObject<ASTLiteralValueNode> ();
    result->sourcePosition = requiredSourcePosition;
    result->value = selfFromThis();
    result->type = getType();
    return result;
}

SSAValuePtr AnyValue::asSSAValueRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition)
{
    return SSAConstantLiteralValue::makeWith(selfFromThis(), getType(), requiredSourcePosition);
}

AnyValuePtr AnyValue::asUnarySelectorConvertedToIdentifier() const
{
    return nullptr;
}

ASTNodePtr AnyValue::analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->analyzeNodeIfNeededWithCurrentExpectedType(asASTNodeRequiredInPosition(partiallyAnalyzedNode->sourcePosition));
}

ASTNodePtr AnyValue::concretizeEphemeralCompileTimeObject(const ASTLiteralValueNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->recordSemanticErrorInNode(partiallyAnalyzedNode, formatString("Cannot concretize ephemeral compile time object ({0}).", {{printString()}}));
}

bool AnyValue::unwrapAsBoolean() const
{
    signalNew<CannotUnwrap> ();
}

uint8_t AnyValue::unwrapAsUInt8() const
{
    signalNew<CannotUnwrap> ();
}

int8_t AnyValue::unwrapAsInt8() const
{
    signalNew<CannotUnwrap> ();
}

uint16_t AnyValue::unwrapAsUInt16() const
{
    signalNew<CannotUnwrap> ();
}

int16_t AnyValue::unwrapAsInt16() const
{
    signalNew<CannotUnwrap> ();
}

uint32_t AnyValue::unwrapAsUInt32() const
{
    signalNew<CannotUnwrap> ();
}

int32_t AnyValue::unwrapAsInt32() const
{
    signalNew<CannotUnwrap> ();
}

uint64_t AnyValue::unwrapAsUInt64() const
{
    signalNew<CannotUnwrap> ();
}

int64_t AnyValue::unwrapAsInt64() const
{
    signalNew<CannotUnwrap> ();
}

LargeInteger AnyValue::unwrapAsLargeInteger() const
{
    signalNew<CannotUnwrap> ();
}

Fraction AnyValue::unwrapAsFraction() const
{
    signalNew<CannotUnwrap> ();
}

float AnyValue::unwrapAsFloat32() const
{
    signalNew<CannotUnwrap> ();
}

double AnyValue::unwrapAsFloat64() const
{
    signalNew<CannotUnwrap> ();
}

char AnyValue::unwrapAsChar8() const
{
    signalNew<CannotUnwrap> ();
}

char16_t AnyValue::unwrapAsChar16() const
{
    signalNew<CannotUnwrap> ();
}

char32_t AnyValue::unwrapAsChar32() const
{
    signalNew<CannotUnwrap> ();
}

std::string AnyValue::unwrapAsString() const
{
    signalNew<CannotUnwrap> ();
}

AnyValuePtrList AnyValue::unwrapAsArray() const
{
    signalNew<CannotUnwrap> ();
}

ASTNodePtr AnyValue::analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->recordSemanticErrorInNode(partiallyAnalyzedNode, formatString("Call node cannot be analyzed by compile time value ({0}).", {{printString()}}));
}

ASTNodePtr AnyValue::analyzeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer)
{
    return semanticAnalyzer->recordSemanticErrorInNode(partiallyAnalyzedNode, formatString("Message send node cannot be analyzed by compile time value ({0}).", {{printString()}}));
}

AnyValuePtr AnyValue::runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
{
    (void)selector;
    (void)receiver;
    (void)arguments;
    signalNew<CannotEvaluateMessage> ();
}

AnyValuePtr AnyValue::applyWithArguments(const std::vector<AnyValuePtr> &arguments)
{
    (void)arguments;
    signalNew<CannotEvaluateMessage> ();
}

AnyValuePtr AnyValue::applyInClosureWithArguments(const AnyValuePtr &closure, const std::vector<AnyValuePtr> &arguments)
{
    (void)closure;
    (void)arguments;
    signalNew<CannotEvaluateMessage> ();
}

AnyValuePtr AnyValue::performWithArguments(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments)
{
    auto type = getType();
    if(!type)
        signalNewWithMessage<MessageNotUnderstood> ("Message " + selector->printString() + " is not understood by " + printString() + ".");
    return type->runWithArgumentsIn(selector, arguments, selfFromThis());
}

AnyValuePtr AnyValue::accessVariableAsReferenceWithType(const TypePtr &referenceType)
{
    (void)referenceType;
    return selfFromThis();
}

AnyValuePtr AnyValue::accessVariableAsValueWithType(const TypePtr &valueType)
{
    (void)valueType;
    return selfFromThis();
}

AnyValuePtr AnyValue::copyAssignValue(const AnyValuePtr &newValue)
{
    (void)newValue;
    return selfFromThis();
}

AnyValuePtr AnyValue::moveAssignValue(const AnyValuePtr &newValue)
{
    (void)newValue;
    return copyAssignValue(newValue);
}

AnyValuePtr AnyValue::asMutableStoreValue()
{
    auto box = basicMakeObject<ValueBox> ();
    box->value = selfFromThis();
    box->type = getType();
    return box;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius