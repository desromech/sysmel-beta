#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ANY_VALUE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ANY_VALUE_HPP
#pragma once

#include <memory>
#include <string>
#include <vector>
#include "LargeInteger.hpp"
#include "Fraction.hpp"
#include "SExpression.hpp"
#include "TypeInferenceMode.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

#define SYSMEL_DECLARE_BOOTSTRAP_CLASS(className) \
    class className; \
    typedef std::shared_ptr<className> className ##Ptr; \
    typedef std::weak_ptr<className> className ##WeakPtr;

#define SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(className) \
    SYSMEL_DECLARE_BOOTSTRAP_CLASS(className) \
    typedef std::vector<className ##Ptr> className ##PtrList; \
    typedef std::vector<className ##WeakPtr> className ##WeakPtrList;

SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(AnyValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(Type);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(FunctionalType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(FunctionType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(MethodType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(ClosureType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(ASTNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(MethodDictionary);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTBuilder);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(MacroInvocationContext);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTCallNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMessageSendNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTLiteralValueNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTIdentifierReferenceNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSemanticAnalyzer);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSourcePosition);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTAnalysisEnvironment);

typedef std::pair<AnyValuePtr, AnyValuePtr> MethodBinding;
typedef std::vector<MethodBinding> MethodBindings;

typedef std::pair<std::string, MethodBindings> MethodCategory;
typedef std::vector<MethodCategory> MethodCategories;

typedef int32_t PatternMatchingRank;

/**
 * I hold the metadata that is required by a bootstrap defined type.
 */
struct StaticBootstrapDefinedTypeMetadata
{
    const StaticBootstrapDefinedTypeMetadata *supertype;
    std::string typeName;
    MethodCategories (*instanceMethods)();
    MethodCategories (*typeMethods)();
    AnyValuePtrList (*constructors)();
    AnyValuePtrList (*conversions)();
    MethodCategories (*instanceMacroMethods)();
    MethodCategories (*typeMacroMethods)();
    AnyValuePtr (*basicNewValue)();
    void (*addTypeConversionRules)(const TypePtr &type);
    bool (*canBeInstantiatedWithLiteralValue)(const AnyValuePtr &value);
    AnyValuePtr (*instantiateWithLiteralValue)(const AnyValuePtr &value);
    TypePtr (*asInferredTypeForWithModeInEnvironment)(const TypePtr &selfType, const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment);
    bool isDynamicCompileTimeType;
    bool isLiteralValueMessageAnalyzer;
    bool isEphemeralCompileTimeObject;
    bool isNullableType;
    bool isImmutableType;
    bool hasTrivialInitialization;
    bool hasTrivialFinalization;
    bool hasTrivialCopyingFrom;
    bool hasTrivialMovingFrom;
    std::string sysmelLanguageTopLevelName;
    size_t bootstrapTypeID;
};

TypePtr getBootstrapDefinedTypeWithID(size_t bootstrapTypeID);

/**
 * Retrieves the singleton nil constant.
 */
AnyValuePtr getNilConstant();

template<typename T>
struct StaticBootstrapDefinedTypeMetadataFor
{
    static StaticBootstrapDefinedTypeMetadata metadata;

    static StaticBootstrapDefinedTypeMetadata *get()
    {
        return &metadata;
    }
};

template<typename T>
StaticBootstrapDefinedTypeMetadata StaticBootstrapDefinedTypeMetadataFor<T>::metadata = StaticBootstrapDefinedTypeMetadata{
    StaticBootstrapDefinedTypeMetadataFor<typename T::SuperType>::get(),
    T::__typeName__,
    &T::__instanceMethods__,
    &T::__typeMethods__,
    &T::__constructors__,
    &T::__conversions__,
    &T::__instanceMacroMethods__,
    &T::__typeMacroMethods__,
    &T::__basicNewValue__,
    &T::__addTypeConversionRules__,
    &T::__canBeInstantiatedWithLiteralValue__,
    &T::__instantiateWithLiteralValue__,
    &T::__asInferredTypeForWithModeInEnvironment__,
    T::__isDynamicCompileTimeType__,
    T::__isLiteralValueMessageAnalyzer__,
    T::__isEphemeralCompileTimeObject__,
    T::__isNullableType__,
    T::__isImmutableType__,
    T::__hasTrivialInitialization__,
    T::__hasTrivialFinalization__,
    T::__hasTrivialCopyingFrom__,
    T::__hasTrivialMovingFrom__,
    T::__sysmelTypeName__,
    0
};

template<>
struct StaticBootstrapDefinedTypeMetadataFor<void>
{
    static StaticBootstrapDefinedTypeMetadata *get()
    {
        return nullptr;
    }
};

template<typename T>
struct StaticBootstrapDefinedTypeFor
{
    static TypePtr get()
    {
        return getBootstrapDefinedTypeWithID(StaticBootstrapDefinedTypeMetadataFor<T>::get()->bootstrapTypeID);
    }
};

template<typename SuT, typename ST>
class SubtypeOf : public SuT
{
public:
    typedef SuT SuperType;
    typedef ST SelfType;

    static constexpr char const __typeName__[] = "";
    static constexpr char const __sysmelTypeName__[] = "";

    static MethodCategories __instanceMethods__()
    {
        return MethodCategories{};
    }

    static MethodCategories __typeMethods__()
    {
        return MethodCategories{};
    }

    static AnyValuePtrList __constructors__()
    {
        return AnyValuePtrList{};
    }

    static AnyValuePtrList __conversions__()
    {
        return AnyValuePtrList{};
    }

    static MethodCategories __instanceMacroMethods__()
    {
        return MethodCategories{};
    }

    static MethodCategories __typeMacroMethods__()
    {
        return MethodCategories{};
    }

    static TypePtr __staticType__()
    {
        return StaticBootstrapDefinedTypeFor<SelfType>::get();
    }

    static AnyValuePtr __basicNewValue__()
    {
        if constexpr(std::is_constructible<SelfType>::value)
            return std::make_shared<SelfType> ();
        else
            return getNilConstant();
    }

    virtual TypePtr getType() const override
    {
        return StaticBootstrapDefinedTypeFor<SelfType>::get();
    }

    std::shared_ptr<SelfType> shared_from_this()
    {
        return std::static_pointer_cast<SelfType> (SuperType::shared_from_this());
    }
};

template<typename SuT, typename ST>
class SubMetaTypeOf : public SuT
{
public:
    typedef SuT SuperType;
    typedef ST SelfType;

    static constexpr char const __typeName__[] = "";
    static constexpr char const __sysmelTypeName__[] = "";

    std::shared_ptr<SelfType> shared_from_this()
    {
        return std::static_pointer_cast<SelfType> (SuperType::shared_from_this());
    }
};

template<typename T>
struct WrapperTypeFor;

template<typename T>
struct WrapValue;

template<typename T>
struct UnwrapValue;

/**
 * Constructs a literal symbol from a string.
 */
AnyValuePtr internSymbol(const std::string &symbolValue);

/**
 * Retrieves the singleton void constant.
 */
AnyValuePtr getVoidConstant();

/**
 * Retrieves the singleton true constant.
 */
AnyValuePtr getTrueConstant();

/**
 * Retrieves the singleton false constant.
 */
AnyValuePtr getFalseConstant();

/**
 * Retrieves the singleton compilation error value constant.
 */
AnyValuePtr getCompilationErrorValueConstant();


/**
 * Retrieves or create a new function type.
 */
FunctionTypePtr getOrCreateFunctionType(const TypePtr &resultType, const TypePtrList &arguments);

/**
 * Retrieves or create a new method type.
 */
MethodTypePtr getOrCreateMethodType(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &arguments);

/**
 * Retrieves or create a new closure type.
 */
ClosureTypePtr getOrCreateClosureType(const TypePtr &resultType, const TypePtrList &arguments);

/**
 * I am the base interface for any value that is passed through the interpreter.
 */
class AnyValue : public std::enable_shared_from_this<AnyValue>
{
public:
    typedef void SuperType;
    typedef AnyValue SelfType;

    static constexpr char const __typeName__[] = "AnyValue";
    static constexpr char const __sysmelTypeName__[] = "AnyValue";
    static constexpr bool __isDynamicCompileTimeType__ = true;
    static constexpr bool __isLiteralValueMessageAnalyzer__ = false;
    static constexpr bool __isEphemeralCompileTimeObject__ = false;
    static constexpr bool __isNullableType__ = true;
    static constexpr bool __isImmutableType__ = false;
    static constexpr bool __hasTrivialInitialization__ = false;
    static constexpr bool __hasTrivialFinalization__ = false;
    static constexpr bool __hasTrivialCopyingFrom__ = false;
    static constexpr bool __hasTrivialMovingFrom__ = false;

    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMethods__();
    static AnyValuePtrList __constructors__();
    static AnyValuePtrList __conversions__();

    static MethodCategories __instanceMacroMethods__();
    static MethodCategories __typeMacroMethods__();
    static void __addTypeConversionRules__(const TypePtr &type);
    
    static TypePtr __staticType__()
    {
        return StaticBootstrapDefinedTypeFor<SelfType>::get();
    }

    static AnyValuePtr __basicNewValue__();
    static bool __canBeInstantiatedWithLiteralValue__(const AnyValuePtr &value);
    static AnyValuePtr __instantiateWithLiteralValue__(const AnyValuePtr &value);
    static TypePtr __asInferredTypeForWithModeInEnvironment__(const TypePtr &selfType, const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment);

    /// Retrieves the type of the object.
    virtual TypePtr getType() const
    {
        return StaticBootstrapDefinedTypeFor<SelfType>::get();
    }

    /// Is this a kind of the specified type?
    virtual bool isKindOf(const TypePtr &type) const;

    template<typename T>
    bool isKindOf() const
    {
        return isKindOf(StaticBootstrapDefinedTypeFor<T>::get());
    }

    virtual ~AnyValue();

    /// Generic method for initializing the object.
    virtual void initialize();

    /// Is this object a program entity?
    virtual bool isCompilerObject() const;

    /// Is this object an AST node?
    virtual bool isASTNode() const;

    /// Is this object an AST builder?
    virtual bool isASTBuilder() const;

    /// Is this object an AST argument definition node?
    virtual bool isASTArgumentDefinitionNode() const;

    /// Is this object an AST identifier reference node?
    virtual bool isASTIdentifierReferenceNode() const;

    /// Is this object an AST intrinsic operation node?
    virtual bool isASTCallNode() const;

    /// Is this object an AST message a make tuple node?
    virtual bool isASTMakeTupleNode() const;

    /// Is this object an AST message a make literal array node?
    virtual bool isASTMakeLiteralArrayNode() const;

    /// Is this object an AST message a make dictionary node?
    virtual bool isASTMakeDictionaryNode() const;

    /// Is this object an AST message a make association node?
    virtual bool isASTMakeAssociationNode() const;

    /// Is this object an AST message chain node?
    virtual bool isASTMessageChainNode() const;

    /// Is this object an AST single message in a chain node?
    virtual bool isASTMessageChainMessageNode() const;

    /// Is this object an AST message send node?
    virtual bool isASTMessageSendNode() const;

    /// Is this object an AST literal value node?
    virtual bool isASTLiteralValueNode() const;

    /// Is this object an AST sequence node?
    virtual bool isASTSequenceNode() const;

    /// Is this object an AST cleanup scope node?
    virtual bool isASTCleanUpScopeNode() const;

    /// Is this object an AST cleanup scope node?
    virtual bool isASTLexicalScopeNode() const;

    /// Is this object an AST cleanup scope node?
    virtual bool isASTClosureNode() const;

    /// Is this object an AST error node?
    virtual bool isASTErrorNode() const;

    /// Is this object an AST parse error node?
    virtual bool isASTParseErrorNode() const;

    /// Is this object an AST semantic error node?
    virtual bool isASTSemanticErrorNode() const;

    /// Is this object an AST compile time error node?
    virtual bool isASTCompileTimeEvaluationErrorNode() const;

    /// Is this object an AST pragma node?
    virtual bool isASTPragmaNode() const;

    /// Is this object an AST quasi-quote node?
    virtual bool isASTQuasiQuoteNode() const;

    /// Is this object an AST quasi-unquote node?
    virtual bool isASTQuasiUnquoteNode() const;

    /// Is this object an AST quote node?
    virtual bool isASTQuoteNode() const;

    /// Is this object an AST source code?
    virtual bool isASTSourceCode() const;

    /// Is this object an AST source position?
    virtual bool isASTSourcePosition() const;

    /// Is this object an empty AST source position?
    virtual bool isASTEmptySourcePosition() const;

    /// Is this object an AST source code position?
    virtual bool isASTSourceCodePosition() const;

    /// Is this object an AST splice node?
    virtual bool isASTSpliceNode() const;

    /// Is this object an AST program entity node?
    virtual bool isASTProgramEntityNode() const;

    /// Is this object an AST variable node?
    virtual bool isASTVariableNode() const;

    /// Is this object an AST local variable node?
    virtual bool isASTLocalVariableNode() const;

    /// Is this object an AST global variable node?
    virtual bool isASTGlobalVariableNode() const;

    /// Is this object an AST field variable node?
    virtual bool isASTFieldVariableNode() const;

    /// Is this object an AST field variable node?
    virtual bool isASTCompileTimeConstantNode() const;

    /// Is this object an AST local immutable access node?
    virtual bool isASTLocalImmutableAccessNode() const;

    /// Is this object an AST variable access node?
    virtual bool isASTVariableAccessNode() const;

    /// Is this object an AST functional node?
    virtual bool isASTFunctionalNode() const;

    /// Is this object an AST function node?
    virtual bool isASTFunctionNode() const;

    /// Is this object an AST method node?
    virtual bool isASTMethodNode() const;

    /// Is this object an AST template node?
    virtual bool isASTTemplateNode() const;

    /// Is this object an AST namespace node?
    virtual bool isASTNamespaceNode() const;

    /// Is this object an AST type node?
    virtual bool isASTTypeNode() const;

    /// Is this object an AST struct node?
    virtual bool isASTStructNode() const;

    /// Is this object an AST union node?
    virtual bool isASTUnionNode() const;

    /// Is this object an AST class node?
    virtual bool isASTClassNode() const;

    /// Is this object an AST enum node?
    virtual bool isASTEnumNode() const;

    /// Is this object an AST program entity extension node?
    virtual bool isASTProgramEntityExtensionNode() const;

    /// Is this object an AST cast node?
    virtual bool isASTCastNode() const;

    /// Is this object an AST implicit cast node?
    virtual bool isASTImplicitCastNode() const;

    /// Is this object an AST implicit cast node?
    virtual bool isASTExplicitCastNode() const;

    /// Is this object an AST implicit cast node?
    virtual bool isASTReinterpretCastNode() const;

    /// Is this object an AST type conversion node?
    virtual bool isASTTypeConversionNode() const;

    /// Is this object an AST value as void type conversion node?
    virtual bool isASTValueAsVoidTypeConversionNode() const;

    /// Is this object an AST upcast type conversion node?
    virtual bool isASTUpcastTypeConversionNode() const;

    /// Is this object an AST downcast type conversion node?
    virtual bool isASTDowncastTypeConversionNode() const;

    /// Is this object an AST if node?
    virtual bool isASTIfNode() const;

    /// Is this object an AST while node?
    virtual bool isASTWhileNode() const;

    /// Is this object an AST do while node?
    virtual bool isASTDoWhileNode() const;

    /// Is this object an AST return node?
    virtual bool isASTReturnNode() const;

    /// Is this object an AST break node?
    virtual bool isASTBreakNode() const;

    /// Is this object an AST continue node?
    virtual bool isASTContinueNode() const;

    /// Is this object an identifier lookup scope?
    virtual bool isIdentifierLookupScope() const;

    /// Is this object clean-up scope?
    virtual bool isCleanUpScope() const;

    /// Is this object a compile time clean-up scope?
    virtual bool isCompileTimeCleanUpScope() const;

    /// Is this object a program entity?
    virtual bool isProgramEntity() const;

    /// Is this object a type?
    virtual bool isType() const;

    /// Is this object a simple type?
    virtual bool isSimpleType() const;

    /// Is this object a meta type?
    virtual bool isMetaType() const;

    /// Is this object a program entity?
    virtual bool isPragma() const;

    /// Is this object a bootstrap defined type?
    virtual bool isBootstrapType() const;

    /// Is this object a macro invocation context?
    virtual bool isMacroInvocationContext() const;

    /// Is this object a macro method?
    virtual bool isMacroMethod() const;

    /// Is this object a method?
    virtual bool isMethod() const;

    /// Is this object a method dictionary?
    virtual bool isMethodDictionary() const;

    /// Is this object a bootstrap method?
    virtual bool isSpecificMethod() const;

    /// Is this object a method that can be evaluated in compile time?
    virtual bool isCompileTimeEvaluableMethod() const;

    /// Is this object a compiled method?
    virtual bool isCompiledMethod() const;

    /// Is this object a bootstrap method?
    virtual bool isBootstrapMethod() const;

    /// Is this object a pattern method?
    virtual bool isPatternMatchingMethod() const;

    /// Is this object a template method?
    virtual bool isTemplateMethod() const;

    /// Is this object a namespace?
    virtual bool isNamespace() const;

    /// Is this object a meta builder?
    virtual bool isMetaBuilder() const;

    /// Is this object a variable?
    virtual bool isVariable() const;

    /// Is this object an argument variable?
    virtual bool isArgumentVariable() const;

    /// Is this object a local variable?
    virtual bool isLocalVariable() const;

    /// Is this object a field variable?
    virtual bool isFieldVariable() const;

    /// Is this object a global variable?
    virtual bool isGlobalVariable() const;

    /// Is this object a literal value?
    virtual bool isLiteralValue() const;

    /// Is this object a literal array?
    virtual bool isLiteralArray() const;

    /// Is this object a literal number?
    virtual bool isLiteralNumber() const;

    /// Is this object a literal integer?
    virtual bool isLiteralInteger() const;

    /// Is this object a literal positive integer?
    virtual bool isLiteralPositiveInteger() const;

    /// Is this object a literal negative integer?
    virtual bool isLiteralNegativeInteger() const;

    /// Is this object a literal character?
    virtual bool isLiteralCharacter() const;

    /// Is this object a literal fraction?
    virtual bool isLiteralFraction() const;

    /// Is this object a literal float?
    virtual bool isLiteralFloat() const;

    /// Is this object a literal string?
    virtual bool isLiteralString() const;

    /// Is this object a literal symbol?
    virtual bool isLiteralSymbol() const;

    /// Is this object a literal identifier symbol?
    virtual bool isLiteralIdentifierSymbol() const;

    /// Is this object a literal boolean?
    virtual bool isLiteralBoolean() const;

    /// Is this object a literal boolean true?
    virtual bool isLiteralBooleanTrue() const;

    /// Is this object a literal boolean false?
    virtual bool isLiteralBooleanFalse() const;

    /// Is this the void object?
    virtual bool isVoid() const;

    /// Is this object undefined?
    virtual bool isUndefined() const;

    /// Is this object a compilation error value?
    virtual bool isCompilationErrorValue() const;

    /// Is this a pure compile time literal value?
    virtual bool isPureCompileTimeLiteralValue() const;

    /// Is this object an anonymouse name?
    virtual bool isAnonymousNameSymbol() const;

    // Is this object a value box?
    virtual bool isValueBox() const;

    // Is this a functional type?
    virtual bool isFunctionalType() const;

    // Is this a functional type value?
    virtual bool isFunctionalTypeValue() const;

    // Is this a function type?
    virtual bool isFunctionType() const;

    // Is this a function type value?
    virtual bool isFunctionTypeValue() const;

    // Is this a method type?
    virtual bool isMethodType() const;

    // Is this a method type value?
    virtual bool isMethodTypeValue() const;

    // Is this a closure type?
    virtual bool isClosureType() const;

    // Is this a closure type value?
    virtual bool isClosureTypeValue() const;

    // Is this a primitive value?
    virtual bool isPrimitiveTypeValue() const;

    // Is this a primitive tensor type?
    virtual bool isPrimitiveTensorTypeValue() const;

    // Is this a primitive scalar type?
    virtual bool isPrimitiveScalarTypeValue() const;

    // Is this a primitive boolean type?
    virtual bool isPrimitiveBooleanTypeValue() const;

    // Is this a primitive number type?
    virtual bool isPrimitiveNumberTypeValue() const;

    // Is this a primitive integer type?
    virtual bool isPrimitiveIntegerTypeValue() const;

    // Is this a primitive integer type?
    virtual bool isPrimitiveCharacterTypeValue() const;

    // Is this a primitive integer type?
    virtual bool isPrimitiveFloatTypeValue() const;

    // Is this an aggregate type?
    virtual bool isAggregateType() const;

    // Is this an aggregate type value?
    virtual bool isAggregateTypeValue() const;

    // Is this an aggregate type with fields?
    virtual bool isAggregateTypeWithFields() const;

    // Is this an aggregate type value?
    virtual bool isAggregateTypeWithFieldsValue() const;

    // Is this a structure type?
    virtual bool isStructureType() const;

    // Is this a structure type value?
    virtual bool isStructureTypeValue() const;

    // Is this a class type?
    virtual bool isClassType() const;

    // Is this a class type value?
    virtual bool isClassTypeValue() const;

    // Is this an union type?
    virtual bool isUnionType() const;

    // Is this an union type value?
    virtual bool isUnionTypeValue() const;

    // Is this a tuple type?
    virtual bool isTupleType() const;

    // Is this a tuple value?
    virtual bool isTupleTypeValue() const;

    // Is this a variant type?
    virtual bool isVariantType() const;

    // Is this a variant type value?
    virtual bool isVariantTypeValue() const;

    // Is this a derived type?
    virtual bool isDerivedType() const;

    // Is this a derived type value?
    virtual bool isDerivedTypeValue() const;

    // Is this a decorated type?
    virtual bool isDecoratedType() const;

    // Is this a decorated type value?
    virtual bool isDecoratedTypeValue() const;

    // Is this a const type?
    virtual bool isConstDecoratedType() const;

    // Is this a volatile type?
    virtual bool isVolatileDecoratedType() const;

    // Is this a restrict derived type?
    virtual bool isRestrictDecoratedType() const;

    // Is this an enum type?
    virtual bool isEnumType() const;

    // Is this an enum type value?
    virtual bool isEnumTypeValue() const;

    // Is this a pointer like type?
    virtual bool isPointerLikeType() const;

    // Is this a pointer like type value?
    virtual bool isPointerLikeTypeValue() const;

    // Is this a pointer like type?
    virtual bool isReferenceLikeType() const;

    // Is this a pointer like type value?
    virtual bool isReferenceLikeTypeValue() const;

    // Is this a pointer type?
    virtual bool isPointerType() const;

    // Is this a pointer type value?
    virtual bool isPointerTypeValue() const;

    // Is this a reference type?
    virtual bool isReferenceType() const;

    // Is this a reference type value?
    virtual bool isReferenceTypeValue() const;

    // Is this a temporary reference type?
    virtual bool isTemporaryReferenceType() const;

    // Is this a temporary reference type value?
    virtual bool isTemporaryReferenceTypeValue() const;

    // Is this a compile time constant?
    virtual bool isCompileTimeConstant() const;

    // Is this a template?
    virtual bool isTemplate() const;

    // Is this a template instance?
    virtual bool isTemplateInstance() const;

    /// Convert the object into a string.
    virtual std::string asString() const;

    /// Convert the object into a string for printing purposes.
    virtual std::string printString() const;

    /// Convert the object into a string for printing purposes.
    virtual std::string fullPrintString() const;

    /// Convert the object into a SExpression.
    virtual SExpression asSExpression() const;

    /// Convert the object into a SExpression with its full definition.
    virtual SExpression asFullDefinitionSExpression() const;

    /// Convert the object into an AST node which is required in the specified source position.
    virtual ASTNodePtr asASTNodeRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition);

    /// Convert this value from unary selector into an identifier value. Null if the conversion is not valid.
    virtual AnyValuePtr asUnarySelectorConvertedToIdentifier() const;

    /// Reads the wrapped value as boolean.
    virtual bool unwrapAsBoolean() const;

    /// Reads the wrapped value as an UInt8.
    virtual uint8_t unwrapAsUInt8() const;

    /// Reads the wrapped value as an Int8.
    virtual int8_t unwrapAsInt8() const;

    /// Reads the wrapped value as an UInt16.
    virtual uint16_t unwrapAsUInt16() const;

    /// Reads the wrapped value as an Int16.
    virtual int16_t unwrapAsInt16() const;

    /// Reads the wrapped value as an UInt32.
    virtual uint32_t unwrapAsUInt32() const;

    /// Reads the wrapped value as an Int32.
    virtual int32_t unwrapAsInt32() const;

    /// Reads the wrapped value as an UInt64.
    virtual uint64_t unwrapAsUInt64() const;

    /// Reads the wrapped value as an Int64.
    virtual int64_t unwrapAsInt64() const;

    /// Reads the wrapped value as a large integer.
    virtual LargeInteger unwrapAsLargeInteger() const;

    /// Reads the wrapped value as a fraction.
    virtual Fraction unwrapAsFraction() const;

    /// Reads the wrapped value as a Char8.
    virtual char unwrapAsChar8() const;

    /// Reads the wrapped value as a Char16.
    virtual char16_t unwrapAsChar16() const;

    /// Reads the wrapped value as a Char32.
    virtual char32_t unwrapAsChar32() const;

    /// Reads the wrapped value as a Float32.
    virtual float unwrapAsFloat32() const;

    /// Reads the wrapped value as a Float64.
    virtual double unwrapAsFloat64() const;

    /// Convert the object into a string.
    virtual std::string unwrapAsString() const;

    /// Convert the object into an array.
    virtual AnyValuePtrList unwrapAsArray() const;

    /// This method performs the semantic analysis of a message send node with the specified semantic analyzer.
    virtual ASTNodePtr analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method performs the semantic analysis of a message send node with the specified semantic analyzer.
    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method performs the semantic analysis of an identifier reference with the specified semantic analyzer.
    virtual ASTNodePtr analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method performs the concretion of an ephemeral compile time object.
    virtual ASTNodePtr concretizeEphemeralCompileTimeObject(const ASTLiteralValueNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method evaluates the specific message in the receiver with the specific arguments.
    virtual AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver);

    /// Evaluates the requested message with the given arguments.
    virtual AnyValuePtr performWithArguments(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments);

    /// Applies the receiver as a function with the given arguments.
    virtual AnyValuePtr applyWithArguments(const std::vector<AnyValuePtr> &arguments);

    /// Applies the receiver as a function in the given closure with the arguments.
    virtual AnyValuePtr applyInClosureWithArguments(const AnyValuePtr &closure, const std::vector<AnyValuePtr> &arguments);

    // Access the value box (e.g. variable binding slot) as a reference with the specified type.
    virtual AnyValuePtr accessVariableAsReferenceWithType(const TypePtr &referenceType);

    // Access the value box (e.g. variable binding slot) as a value with the specified type.
    virtual AnyValuePtr accessVariableAsValueWithType(const TypePtr &valueType);

    // Wraps and/or copy the object to make a mutable version of it.
    virtual AnyValuePtr asMutableStoreValue();

    // Assigns a new value onto myself.
    virtual AnyValuePtr copyAssignValue(const AnyValuePtr &newValue);

    // Assigns a new value onto myself.
    virtual AnyValuePtr moveAssignValue(const AnyValuePtr &newValue);

    template<typename ResultType, typename... Args>
    ResultType perform(const AnyValuePtr &selector, Args&& ...arguments)
    {
        std::vector<AnyValuePtr> marshalledArguments = {
            WrapValue<Args>::apply(std::forward<Args> (arguments))...
        };
        return UnwrapValue<ResultType>::apply (performWithArguments(selector, marshalledArguments));
    }

    template<typename ResultType, typename... Args>
    ResultType perform(const std::string &selector, Args&& ...arguments)
    {
        return perform<ResultType> (internSymbol(selector), arguments...);
    }

};

inline AnyValuePtr validAnyValue(const AnyValuePtr &value)
{
    return value ? value : getNilConstant();
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ANY_VALUE_HPP