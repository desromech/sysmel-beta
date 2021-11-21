#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"
#include <functional>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(SpecificMethod);
SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(TypeConversionRule);

typedef std::function<void (TypePtr)> TypeIterationBlock;

/**
 * I am the base interface for all of the types that are defined in the system.
 */
class Type : public SubtypeOf<ModuleDefinedProgramEntity, Type>
{
public:
    static constexpr char const __typeName__[] = "Type";
    static constexpr char const __sysmelTypeName__[] = "Type";

    static MethodCategories __instanceMethods__();
    static MethodCategories __instanceMacroMethods__();

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

    static TypePtr getPragmaType();
    static TypePtr getArgumentVariableType();
    static TypePtr getReturnType();
    static TypePtr getCompilationErrorValueType();

    bool isUndefinedType() const;
    bool isVoidType() const;
    bool isAutoType() const;
    bool isReturnType() const;
    bool isLiteralValueType() const;
    bool isCompilationErrorValueType() const;

    /// This method evaluates a specific message in the receiver with the specific arguments.
    virtual TypePtr getSupertype() const;

    /// This method sets the super type.
    virtual void setSupertype(const TypePtr &newSupertype);

    /// This method retrieves the subtypes.
    virtual const TypePtrList &getSubtypes();

    /// This method registers a subtypes.
    virtual void registerSubtype(const TypePtr &subtype);

    /// Converts this type into a specific receiver type.
    virtual TypePtr asReceiverType();

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

    /// Does this type support dynamic compile time message sends?
    virtual bool supportsDynamicCompileTimeMessageSend() const;

    /// Does this type support delegating the analysis of messages into literal value receivers?
    virtual bool supportsMessageAnalysisByLiteralValueReceivers() const;

    /// Is this a type that should be concretized always?
    virtual bool isEphemeralCompileTimeObject() const;

    /// Is this a whose value can be null?
    virtual bool isNullableType() const;

    /// Is this a type with a trivial initialization?
    virtual bool hasTrivialInitialization() const;

    /// Is this a type with a trivial finalization?
    virtual bool hasTrivialFinalization() const;

    /// Is this a type with a trivial copy process?
    virtual bool hasTrivialCopyingFrom() const;

    /// Is this a type with a trivial movement process?
    virtual bool hasTrivialMovingFrom() const;

    /// This method performs the semantic analysis of a call node with the specified semantic analyzer.
    virtual ASTNodePtr analyzeCallNode(const ASTCallNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method performs the semantic analysis of a message send node with the specified semantic analyzer.
    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    /// This method performs the semantic analysis of a message send node with the specified semantic analyzer.
    virtual ASTNodePtr analyzeMessageSendNodeWithTypeDefinedMethods(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method performs the semantic analysis of a message send node whose selector was not found.
    virtual ASTNodePtr analyzeUnboundMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method performs the concretion of an ephemeral compile time object.
    virtual ASTNodePtr concretizeEphemeralCompileTimeObject(const ASTLiteralValueNodePtr &node, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method evaluates a specific message in the receiver with the specific arguments.
    virtual AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver) override;

    /// This method add a new constructor function.
    virtual void addConstructor(const AnyValuePtr &constructorMethod);

    /// This method adds a list of constructors.
    virtual void addConstructors(const AnyValuePtrList &constructorMethods);

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
    virtual TypeConversionRulePtr findImplicitTypeConversionRuleForInto(const ASTNodePtr &node, const TypePtr &targetType);

    /// This method finds an explicit type conversion rule.
    virtual TypeConversionRulePtr findExplicitTypeConversionRuleForInto(const ASTNodePtr &node, const TypePtr &targetType);

    /// This method finds an reinterpret type conversion rule.
    virtual TypeConversionRulePtr findReinterpretTypeConversionRuleForInto(const ASTNodePtr &node, const TypePtr &targetType);

    /// This method adds the default type conversion rules of this type.
    virtual void addDefaultTypeConversionRules();

    /// Adds an explicit and implicit type conversion rule.
    virtual void addTypeConversionRule(const TypeConversionRulePtr &rule);

    /// Adds an explicit type conversion rule.
    virtual void addExplicitTypeConversionRule(const TypeConversionRulePtr &rule);

    /// Adds an implicit type conversion rule.
    virtual void addImplicitTypeConversionRule(const TypeConversionRulePtr &rule);

    /// Adds an reinterpret type conversion rule.
    virtual void addReinterpretTypeConversionRule(const TypeConversionRulePtr &rule);

    /// Expands the #basicNewValue macro.
    virtual ASTNodePtr expandBasicNewValue(const MacroInvocationContextPtr &context);

    /// Expands the #newValue macro.
    virtual ASTNodePtr expandNewValue(const MacroInvocationContextPtr &context);

    /// Constructs a value of this type.
    virtual AnyValuePtr basicNewValue();

    /// Get the default value of this type.
    virtual AnyValuePtr defaultValue();
    
    /// This method analyzes the construction of a value of this type with the specified arguments.
    virtual ASTNodePtr analyzeValueConstructionWithArguments(const ASTNodePtr &node, const ASTNodePtrList &arguments, const ASTSemanticAnalyzerPtr &semanticAnalyzer);
    
    /// Is it possible to instantiate this type with the specified literal value?
    virtual bool canBeInstantiatedWithLiteralValue(const AnyValuePtr &value);

    /// Make an instance with the specified literal value
    virtual AnyValuePtr instantiatedWithLiteralValue(const AnyValuePtr &value);

protected:
    // Utility method for expanding type macros.
    static TypePtr extractTypeForTypeMacroReceiverNode(const ASTNodePtr &receiverNode);

    /// This method evaluates all of the pending code fragments.
    virtual void evaluateAllPendingCodeFragments();

    /// This method evaluates all of the pending body block code fragments.
    virtual void evaluateAllPendingBodyBlockCodeFragments();

    AnyValuePtr name;
    mutable TypePtr supertype;
    TypePtrList subtypes;

    ProgramEntityPtrList children;
    std::unordered_map<AnyValuePtr, ProgramEntityVisibilityWithBinding> bindings;
    
    MethodDictionaryPtr macroMethodDictionary;
    MethodDictionaryPtr methodDictionary;
    MethodDictionaryPtr macroFallbackMethodDictionary;
    SpecificMethodPtrList constructors;

    TypeConversionRulePtrList implicitTypeConversionRules;
    TypeConversionRulePtrList explicitTypeConversionRules;
    TypeConversionRulePtrList reinterpretTypeConversionRules;

    DeferredCompileTimeCodeFragmentPtrList pendingBodyBlockCodeFragments;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_HPP