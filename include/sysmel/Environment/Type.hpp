#ifndef SYSMEL_ENVIRONMENT_TYPE_HPP
#define SYSMEL_ENVIRONMENT_TYPE_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"
#include "TypeDecorationFlags.hpp"
#include <functional>
#include <unordered_map>

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SpecificMethod);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(TypeConversionRule);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PointerType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ReferenceType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(PointerLikeType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ArrayType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(TupleType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSATypeProgramEntity);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(TypeVisitor);

typedef std::function<void (TypePtr)> TypeIterationBlock;

/**
 * I am the base interface for all of the types that are defined in the system.
 */
class SYSMEL_COMPILER_LIB_EXPORT Type : public SubtypeOf<ModuleDefinedProgramEntity, Type>
{
public:
    static constexpr char const __typeName__[] = "Type";
    static constexpr char const __sysmelTypeName__[] = "Type";

    static MethodCategories __instanceMethods__();
    static MethodCategories __instanceMacroMethods__();

    Type();
    
    virtual bool isType() const override;
    virtual std::string printString() const override;
    virtual void setName(const AnyValuePtr &newName);
    virtual AnyValuePtr getName() const override;

    static TypePtr getAnyValueType();
    static TypePtr getLiteralValueType();
    static TypePtr getLiteralSymbolValue();
    static TypePtr getUndefinedType();
    static TypePtr getVoidType();
    static TypePtr getAutoType();
    static TypePtr getNamespaceType();
    static TypePtr getIntPointerType();
    static TypePtr getUIntPointerType();

    static TypePtr getPragmaType();
    static TypePtr getArgumentVariableType();
    static TypePtr getControlFlowEscapeType();
    static TypePtr getReturnType();
    static TypePtr getContinueType();
    static TypePtr getBreakType();
    static TypePtr getCompilationErrorValueType();
    static TypePtr getCVarArgsType();

    bool isUndefinedType() const;
    bool isVoidType() const;
    bool isAutoType() const;
    bool isControlFlowEscapeType() const;
    bool isReturnType() const;
    bool isContinueType() const;
    bool isBreakType() const;
    bool isLiteralValueType() const;
    bool isCompilationErrorValueType() const;
    bool isCVarArgType() const;
    bool isCVarArgsType() const;

    virtual AnyValuePtr acceptLiteralValueVisitor(const LiteralValueVisitorPtr &visitor) override;

    /// Accept a type visitor.
    virtual AnyValuePtr acceptTypeVisitor(const TypeVisitorPtr &visitor);

    /// This method evaluates a specific message in the receiver with the specific arguments.
    virtual TypePtr getSupertype() const;

    /// This method sets the super type.
    virtual void setSupertype(const TypePtr &newSupertype);

    /// This method retrieves the subtypes.
    virtual const TypePtrList &getSubtypes();

    /// This method registers a subtypes.
    virtual void registerSubtype(const TypePtr &subtype);

    /// Converts this type into a specific receiver type.
    virtual TypePtr asConstReceiverType() override;

    /// Converts this type into a specific receiver type.
    virtual TypePtr asReceiverType() override;

    /// This method performs the lookup for a macro message with the specified selector.
    virtual AnyValuePtr lookupMacroSelector(const AnyValuePtr &selector) override;

    /// This method performs the lookup for a macro message with the specified selector only in this type.
    virtual AnyValuePtr lookupLocalMacroSelector(const AnyValuePtr &selector) override;

    /// This method performs the lookup of macro fallback message with the specified selector.
    virtual AnyValuePtr lookupMacroFallbackSelector(const AnyValuePtr &selector) override;

    /// This method performs the lookup for a message with the specified selector.
    virtual AnyValuePtr lookupSelector(const AnyValuePtr &selector) override;

    /// This method performs the lookup for a message with the specified selector only in this type.
    virtual AnyValuePtr lookupLocalSelector(const AnyValuePtr &selector) override;

    /// This method performs the lookup of the doesNotUnderstand: macro
    virtual AnyValuePtr lookupDoesNotUnderstandMacro();

    /// This method performs the lookup for a macro fallback message with the specified selector only in this type.
    virtual AnyValuePtr lookupLocalMacroFallbackSelector(const AnyValuePtr &selector) override;

    virtual AnyValuePtr lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope) override;

    /// Does this type support dynamic compile time message sends?
    virtual bool supportsDynamicCompileTimeMessageSend() const;

    /// Does this type support delegating the analysis of messages into literal value receivers?
    virtual bool supportsMessageAnalysisByLiteralValueReceivers() const;

    /// Is this a type that should be concretized always?
    virtual bool isEphemeralCompileTimeObject() const;

    /// Is this a whose value can be null?
    virtual bool isNullableType() const;

    /// Is this a value that cannot be modified?
    virtual bool isImmutableType();

    /// Is this a gc type?
    virtual bool isGCType();

    /// Is this a type with a separate reference vs stack value?
    virtual bool isSeparateValueInstanceType();

    /// Is this a value that is passed by reference?
    virtual bool isPassedByReference();

    /// Is this a value that is returned by reference?
    virtual bool isReturnedByReference();

    /// Is this a type with a trivial initialization?
    virtual bool hasTrivialInitialization();

    /// Retrieves the initialize method.
    virtual AnyValuePtr getInitializeMethod();

    /// Is this a type with a trivial initialization copying from?
    virtual bool hasTrivialInitializationCopyingFrom();

    /// Retrieves the finalzie method.
    virtual AnyValuePtr getInitializeCopyingFromMethod();

    /// Is this a type with a trivial initialization moving from?
    virtual bool hasTrivialInitializationMovingFrom();

    /// Retrieves the finalzie method.
    virtual AnyValuePtr getInitializeMovingFromMethod();

    /// Is this a type with a trivial finalization?
    virtual bool hasTrivialFinalization();

    /// Retrieves the finalzie method.
    virtual AnyValuePtr getFinalizeMethod();

    /// Is this a type with a trivial copy process?
    virtual bool hasTrivialAssignCopyingFrom();

    /// Retrieves the := (self const ref) method.
    virtual AnyValuePtr getAssignCopyingFromMethod();

    /// Is this a type with a trivial movement process?
    virtual bool hasTrivialAssignMovingFrom();

    /// Retrieves the := (self tempRef) method.
    virtual AnyValuePtr getAssignMovingFromMethod();

    /// Gets the type value size.
    virtual uint64_t getValueSize();

    /// Gets the type value alignment.
    virtual uint64_t getValueAlignment();

    /// Gets the type aligned value size.
    virtual uint64_t getAlignedValueSize();

    /// Gets the type in memory size.
    virtual uint64_t getMemorySize();

    /// Gets the type in memory alignment.
    virtual uint64_t getMemoryAlignment();

    /// Gets the type in aligned memory size.
    virtual uint64_t getAlignedMemorySize();

    /// This method performs the semantic analysis of a call node with the specified semantic analyzer.
    virtual ASTNodePtr analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    /// This method performs the semantic analysis of a message send node with the specified semantic analyzer.
    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    /// This method performs the semantic analysis of a message send node with the specified semantic analyzer.
    virtual ASTNodePtr analyzeMessageSendNodeWithTypeDefinedMethods(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method performs the semantic analysis of a message send node whose selector was not found.
    virtual ASTNodePtr analyzeUnboundMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method performs the concretion of an ephemeral compile time object.
    virtual ASTNodePtr concretizeEphemeralCompileTimeObject(const ASTLiteralValueNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    /// This method evaluates a specific message in the receiver with the specific arguments.
    virtual AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver) override;

    /// This method adds a new constructor method.
    virtual void addConstructor(const AnyValuePtr &constructorMethod);

    /// This method adds a list of constructors.
    virtual void addConstructors(const AnyValuePtrList &constructorMethods);

    /// This method adds a new conversion method.
    virtual void addConversion(const AnyValuePtr &conversionMethod);

    /// This method adds a list of conversion method.
    virtual void addConversions(const AnyValuePtrList &conversionMethods);

    /// This method adds a field variable with the specified visibility.
    virtual void addFieldVariableWithVisibility(const FieldVariablePtr &field, ProgramEntityVisibility visibility) override;

    /// Gets the number of fields defined in this type.
    size_t getFieldCount();

    /// Gets the field at the given index.
    const FieldVariablePtr &getField(size_t index);

    /// Gets the fields defined in this type.
    const FieldVariablePtrList &getFields();

    /// This method add a new macro method into the method dictionary with the specified selector.
    virtual void addMacroMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector) override;

    /// This method add a new method into the method dictionary with the specified selector.
    virtual void addMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector) override;

    /// This method add a new macro method into the method dictionary with the specified selector.
    virtual void addMacroFallbackMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector) override;

    /// Is this type a kind of the other type?
    virtual bool isSubtypeOf(const TypePtr &otherType) const;

    /// This method computes the rank required for matching the specified type without implicit casting.
    virtual PatternMatchingRank rankToMatchType(const TypePtr &type);

    /// This method computes the rank required for matching the specified value without implicit casting.
    virtual PatternMatchingRank rankToMatchValue(const AnyValuePtr &value);

    /// This method returns the instance type.
    virtual TypePtr getInstanceType();

    /// This method returns the meta type.
    virtual TypePtr getMetaType();

    /// This methods iterates through the direct subtypes.
    void subtypesDo(const TypeIterationBlock &aBlock);

    /// This methods iterates through all the direct subtypes.
    void allSubtypesDo(const TypeIterationBlock &aBlock);

    /// This methods iterates through all the direct subtypes and myself.
    void withAllSubtypesDo(const TypeIterationBlock &aBlock);

    /// This method enqueue the analysis of a body block.
    virtual void enqueuePendingBodyBlockCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment) override;

    virtual void ensureSemanticAnalysis() override;

    virtual void recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild) override;

    virtual void bindSymbolWithVisibility(const AnyValuePtr &symbol, ProgramEntityVisibility visibility, const ProgramEntityPtr &binding) override;

    /// This method finds an implicit type conversion rule.
    virtual TypeConversionRulePtr findImplicitTypeConversionRuleForInto(const ASTNodePtr &node, const TypePtr &targetType, bool isReceiverType = false);

    /// This method finds an explicit type conversion rule.
    virtual TypeConversionRulePtr findExplicitTypeConversionRuleForInto(const ASTNodePtr &node, const TypePtr &targetType);

    /// This method finds an reinterpret type conversion rule.
    virtual bool canBeReinterpretedAsType(const TypePtr &otherType);

    /// This method adds the default type conversion rules of this type.
    virtual void addDefaultTypeConversionRules();

    /// Adds an explicit and implicit type conversion rule.
    virtual void addTypeConversionRule(const TypeConversionRulePtr &rule);

    /// Adds an explicit type conversion rule.
    virtual void addExplicitTypeConversionRule(const TypeConversionRulePtr &rule);

    /// Adds an implicit type conversion rule.
    virtual void addImplicitTypeConversionRule(const TypeConversionRulePtr &rule);

    /// Expands the #basicNewValue macro.
    virtual ASTNodePtr expandBasicNewValue(const MacroInvocationContextPtr &context);

    /// Expands the #newValue macro.
    virtual ASTNodePtr expandNewValue(const MacroInvocationContextPtr &context);

    /// Expands the construction of a value.
    virtual ASTNodePtr expandCopyConstruction(const MacroInvocationContextPtr &context, const ASTNodePtr &valueNode);

    /// Expands the movement of a value.
    virtual ASTNodePtr expandMoveConstruction(const MacroInvocationContextPtr &context, const ASTNodePtr &valueNode);

    /// Constructs a value of this type.
    virtual AnyValuePtr basicNewValue();

    /// Constructs a value of this type by cloning an existent value.
    virtual AnyValuePtr shallowCloneInstance(const AnyValuePtr &cloned);

    /// Get the default value of this type.
    virtual AnyValuePtr defaultValue();
    
    /// This method analyzes the construction of a value of this type with the specified arguments.
    virtual ASTNodePtr analyzeValueConstructionWithArguments(const ASTNodePtr &node, const ASTNodePtrList &arguments, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method analyzes the construction of a value of this type with the specified arguments using a fallback method, or error.
    virtual ASTNodePtr analyzeFallbackValueConstructionWithArguments(const ASTNodePtr &node, const ASTNodePtrList &arguments, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// Is it possible to instantiate this type with the specified literal value?
    virtual bool canBeInstantiatedWithLiteralValue(const AnyValuePtr &value);

    /// Make an instance with the specified literal value
    virtual AnyValuePtr instantiatedWithLiteralValue(const AnyValuePtr &value);

    /// Type inference.
    virtual TypePtr asInferredTypeForWithModeInEnvironment(const ASTNodePtr &node, TypeInferenceMode mode, bool isMutable, bool concreteLiterals, const ASTAnalysisEnvironmentPtr &environment);
    
    /// Makes a pointer type where I am the base type.
    virtual PointerTypePtr pointer();

    /// Makes a pointer type where I am the base type with the specified address space.
    virtual PointerTypePtr pointerFor(const AnyValuePtr &addressSpace);

    /// Makes a reference type where I am the base type.
    virtual ReferenceTypePtr ref();

    /// Makes a reference type where I am the base type with the specified address space.
    virtual ReferenceTypePtr refFor(const AnyValuePtr &addressSpace);

    /// Makes a reference type where I am the base type, and I have the same qualifications as another reference type.
    virtual ReferenceTypePtr refForMemberOfReceiverWithType(const TypePtr &receiverType);

    /// Makes a temporary reference type where I am the base type.
    virtual PointerLikeTypePtr tempRef();

    /// Makes a temporary reference type where I am the base type with the specified address space.
    virtual PointerLikeTypePtr tempRefFor(const AnyValuePtr &addressSpace);

    /// Makes a zero-sized array type of myself.
    virtual ArrayTypePtr arrayWithoutSize();

    /// Makes an array type of myself with the specified size.
    virtual ArrayTypePtr arrayWithSize(uint64_t size);

    /// Wraps myself in a tuple type.
    virtual TypePtr asTupleType();

    /// Appends a type for making a tuple.
    virtual TypePtr appendTypeMakingTuple(const TypePtr &nextType);

    /// Appends a type for making a variant.
    virtual TypePtr appendTypeMakingVariant(const TypePtr &nextType);

    /// Appends a type for making a variant.
    virtual TypePtr appendResultTypeMakingFunctionType(const TypePtr &nextType);

    /// Adds the const decoration to this type.
    virtual TypePtr withConst();

    /// Adds the restrict decoration to this type.
    virtual TypePtr withRestrict();

    /// Adds the volatile decoration to this type.
    virtual TypePtr withVolatile();

    /// Adds the params decoration to this type.
    virtual TypePtr withParams();

    /// Adds the specified decorations to this type.
    virtual TypePtr withDecorations(TypeDecorationFlags decorations);

    /// Removes the decorations of this type.
    virtual TypePtr asUndecoratedType();

    /// Removes the decorations and ref/tempRef of this type.
    virtual TypePtr asDecayedType();

    /// Removes the decorations of this that are omitted from an argument type.
    virtual TypePtr asCanonicalArgumentType();

    /// Removes the decorations of this that are omitted from a result type.
    virtual TypePtr asCanonicalResultType();

    /// Is this T const or T const ref?
    virtual bool isConstOrConstReferenceType() const;

    /// T => T const, T ref => T const ref
    virtual TypePtr asConstOrConstReferenceType();

    /// Gets the decoration flags
    virtual TypeDecorationFlags getDecorationFlags() const;

    /// The type SSA value.
    virtual SSAValuePtr asSSAValueRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition) override;

    const ASTSourcePositionPtr &getSourceDefinitionPosition() const;

    void setSourceDefinitionPosition(const ASTSourcePositionPtr &position);

protected:
    // Utility method for expanding type macros.
    static TypePtr extractTypeForTypeMacroReceiverNode(const ASTNodePtr &receiverNode);

    /// This method evaluates all of the pending code fragments.
    virtual void evaluateAllPendingCodeFragments();

    /// This method evaluates all of the pending body block code fragments.
    virtual void evaluateAllPendingBodyBlockCodeFragments();

    /// This method ensures the creation of the implicit life-time methods.
    virtual void ensureImplicitLifeTimeMethodsAreCreated();

    /// Lookup for a valid specific lifetime method.
    AnyValuePtr lookupValidLifetimeMethod(const std::string &selector, const TypePtrList &argumentTypes, const TypePtr &resultType);

    /// Lookup for a specific lifetime method.
    AnyValuePtr lookupLifetimeMethod(const std::string &selector, const TypePtrList &argumentTypes, const TypePtr &resultType);

    /// Has a trivial lifetime method?
    bool hasTrivialLifetimeMethod(const std::string &selector, const TypePtrList &argumentTypes, const TypePtr &resultType);

    void recordPotentialSpecialMethod(const AnyValuePtr &method);

    AnyValuePtr name;
    mutable TypePtr supertype;
    TypePtrList subtypes;

    ProgramEntityPtrList children;
    std::unordered_map<AnyValuePtr, ProgramEntityVisibilityWithBinding> bindings;
    
    FieldVariablePtrList fields;
    MethodDictionaryPtr macroMethodDictionary;
    MethodDictionaryPtr methodDictionary;
    MethodDictionaryPtr macroFallbackMethodDictionary;
    SpecificMethodPtrList constructors;
    SpecificMethodPtrList conversions;

    TypeConversionRulePtrList implicitTypeConversionRules;
    TypeConversionRulePtrList explicitTypeConversionRules;

    DeferredCompileTimeCodeFragmentPtrList pendingBodyBlockCodeFragments;

    SSATypeProgramEntityPtr ssaTypeProgramEntity;

    ASTSourcePositionPtr definitionPosition;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_TYPE_HPP