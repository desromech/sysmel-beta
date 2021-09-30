#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ANY_VALUE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_ANY_VALUE_HPP
#pragma once

#include <memory>
#include <string>
#include <vector>
#include "LargeInteger.hpp"
#include "Fraction.hpp"
#include "SExpression.hpp"

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
    typedef std::vector<className ##Ptr> className ##PtrList;

SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(AnyValue);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(Type);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(ASTNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(MethodDictionary);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTBuilder);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(MacroInvocationContext);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTMessageSendNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTIdentifierReferenceNode);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSemanticAnalyzer);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ASTSourcePosition);

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
    MethodCategories (*instanceMacroMethods)();
    MethodCategories (*typeMacroMethods)();
    size_t bootstrapTypeID;
};

TypePtr getBootstrapDefinedTypeWithID(size_t bootstrapTypeID);

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
    &T::__instanceMacroMethods__,
    &T::__typeMacroMethods__,
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

    static MethodCategories __instanceMethods__()
    {
        return MethodCategories{};
    }

    static MethodCategories __typeMethods__()
    {
        return MethodCategories{};
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

    virtual TypePtr getType() const override
    {
        return StaticBootstrapDefinedTypeFor<SelfType>::get();
    }

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
 * Retrieves the singleton nil constant.
 */
AnyValuePtr getNilConstant();

/**
 * Retrieves the singleton true constant.
 */
AnyValuePtr getTrueConstant();

/**
 * Retrieves the singleton false constant.
 */
AnyValuePtr getFalseConstant();

/**
 * I am the base interface for any value that is passed through the interpreter.
 */
class AnyValue : public std::enable_shared_from_this<AnyValue>
{
public:
    typedef void SuperType;
    typedef AnyValue SelfType;

    static constexpr char const __typeName__[] = "AnyValue";

    static MethodCategories __instanceMethods__();
    static MethodCategories __typeMethods__();

    static MethodCategories __instanceMacroMethods__();
    static MethodCategories __typeMacroMethods__();

    static TypePtr __staticType__()
    {
        return StaticBootstrapDefinedTypeFor<SelfType>::get();
    }

    /// Retrieves the type of the object.
    virtual TypePtr getType() const
    {
        return StaticBootstrapDefinedTypeFor<SelfType>::get();
    }

    /// Is this a kind of the specified type?
    virtual bool isKindOf(const TypePtr &type) const;

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
    virtual bool isASTIntrinsicOperationNode() const;

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

    /// Is this object an identifier lookup scope?
    virtual bool isIdentifierLookupScope() const;

    /// Is this object clean-up scope?
    virtual bool isCleanUpScope() const;

    /// Is this object a program entity?
    virtual bool isProgramEntity() const;

    /// Is this object a type?
    virtual bool isType() const;

    /// Is this object a meta type?
    virtual bool isMetaType() const;

    /// Is this object a basic type?
    virtual bool isBasicType() const;

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

    /// Is this object a compiled method?
    virtual bool isCompiledMethod() const;

    /// Is this object a bootstrap method?
    virtual bool isBootstrapMethod() const;

    /// Is this object a pattern method?
    virtual bool isPatternMatchingMethod() const;

    /// Is this object a template method?
    virtual bool isTemplateMethod() const;

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
    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method performs the semantic analysis of an identifier reference with the specified semantic analyzer.
    virtual ASTNodePtr analyzeIdentifierReferenceNode(const ASTIdentifierReferenceNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method evaluates the specific message in the receiver with the specific arguments.
    virtual AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver);

    /// Evaluates the requested message with the given arguments.
    virtual AnyValuePtr performWithArguments(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments);

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