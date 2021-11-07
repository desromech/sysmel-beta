#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"
#include <functional>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(DeferredCompileTimeCodeFragment);

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

    virtual bool isType() const override;
    virtual std::string printString() const override;
    virtual void setName(const AnyValuePtr &newName);
    virtual AnyValuePtr getName() const override;

    static TypePtr getAnyValueType();
    static TypePtr getLiteralValueType();
    static TypePtr getLiteralSymbolValue();
    static TypePtr getUndefinedType();
    static TypePtr getVoidType();
    static TypePtr getNamespaceType();

    static TypePtr getPragmaType();
    static TypePtr getArgumentVariableType();
    static TypePtr getReturnType();
    static TypePtr getCompilationErrorValueType();

    bool isUndefinedType() const;
    bool isVoidType() const;
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
    virtual void enqueuePendingBodyBlockCodeFragment(const DeferredCompileTimeCodeFragmentPtr &codeFragment);

    virtual void ensureSemanticAnalysis() override;

protected:
    /// This method evaluates all of the pending code fragments.
    virtual void evaluateAllPendingCodeFragments();

    /// This method evaluates all of the pending body block code fragments.
    virtual void evaluateAllPendingBodyBlockCodeFragments();

    AnyValuePtr name;
    mutable TypePtr supertype;
    TypePtrList subtypes;

    MethodDictionaryPtr macroMethodDictionary;
    MethodDictionaryPtr methodDictionary;
    MethodDictionaryPtr macroFallbackMethodDictionary;

    DeferredCompileTimeCodeFragmentPtrList pendingBodyBlockCodeFragments;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_HPP