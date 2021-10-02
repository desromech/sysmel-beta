#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"
#include <functional>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

typedef std::function<void (TypePtr)> TypeIterationBlock;

/**
 * The diferent levels for a message send expansion.
 */
enum class MessageSendExpansionLevel : uint8_t
{
    UnexpandedMacros = 0,
    ExpandedMacros,
    FallbackMacros,

    Count,
};

/**
 * I am the base interface for all of the types that are defined in the system.
 */
class Type : public SubtypeOf<ModuleDefinedProgramEntity, Type>
{
public:
    static constexpr char const __typeName__[] = "Type";

    static MethodCategories __instanceMethods__();

    virtual bool isType() const;

    static TypePtr getLiteralValueType();
    static TypePtr getLiteralSymbolValue();
    static TypePtr getUndefinedType();
    static TypePtr getVoidType();
    static TypePtr getPragmaType();
    static TypePtr getReturnType();

    bool isUndefinedType() const;
    bool isVoidType() const;
    bool isReturnType() const;
    bool isLiteralValueType() const;

    /// This method evaluates a specific message in the receiver with the specific arguments.
    virtual TypePtr getSupertype() const;

    /// This method sets the super type.
    virtual void setSupertype(const TypePtr &newSupertype);

    /// This method retrieves the subtypes.
    virtual const TypePtrList &getSubtypes();

    /// This method registers a subtypes.
    virtual void registerSubtype(const TypePtr &subtype);

    /// This method performs the lookup for a macro message with the specified selector.
    virtual AnyValuePtr lookupMacroSelector(const AnyValuePtr &selector);

    /// This method performs the lookup of macro fallback message with the specified selector.
    virtual AnyValuePtr lookupMacroFallbackSelector(const AnyValuePtr &selector);

    /// This method performs the lookup for a message with the specified selector.
    virtual AnyValuePtr lookupSelector(const AnyValuePtr &selector);

    /// This method performs the lookup for a macro message or message with the specified selector and expansion level.
    virtual AnyValuePtr lookupSelectorInReceiverNodeWithExpansionLevel(const AnyValuePtr &selector, const ASTNodePtr &receiverNode, MessageSendExpansionLevel expansionLevel);

    /// This method performs the lookup of the doesNotUnderstand: macro
    virtual AnyValuePtr lookupDoesNotUnderstandMacro();

    /// Does this type support dynamic compile time message sends?
    virtual bool supportsDynamicCompileTimeMessageSend() const;

    /// This method performs the semantic analysis of a message send node with the specified semantic analyzer.
    virtual ASTNodePtr analyzeMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer) override;

    /// This method performs the semantic analysis of a message send node whose selector was not found.
    virtual ASTNodePtr analyzeUnboundMessageSendNode(const ASTMessageSendNodePtr &partiallyAnalyzedNode, const ASTSemanticAnalyzerPtr &semanticAnalyzer);

    /// This method evaluates a specific message in the receiver with the specific arguments.
    virtual AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver) override;

    /// This method add a new macro method into the method dictionary with the specified selector.
    virtual void addMacroMethodWithSelector(const AnyValuePtr &selector, const AnyValuePtr &method);

    /// This method adds the method in the specified categories.
    virtual void addMacroMethodCategories(const MethodCategories &categories);

    /// This method add a new method into the method dictionary with the specified selector.
    virtual void addMethodWithSelector(const AnyValuePtr &selector, const AnyValuePtr &method);

    /// This method adds the method in the specified categories.
    virtual void addMethodCategories(const MethodCategories &categories);

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

protected:
    virtual AnyValuePtr lookupLocalSelector(const AnyValuePtr &selector);
    virtual AnyValuePtr lookupLocalMacroSelector(const AnyValuePtr &selector);
    virtual AnyValuePtr lookupLocalMacroFallbackSelector(const AnyValuePtr &selector);

    TypePtr supertype;
    TypePtrList subtypes;

    MethodDictionaryPtr macroMethodDictionary;
    MethodDictionaryPtr methodDictionary;
    MethodDictionaryPtr macroFallbackMethodDictionary;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_TYPE_HPP