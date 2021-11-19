#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_HPP
#pragma once

#include "CompilerObject.hpp"
#include "ProgramEntityVisibility.hpp"
#include <utility>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS_AND_LIST(ProgramEntity);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(Module);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(RuntimeContext);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Method);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(Variable);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(IdentifierLookupScope);

typedef std::pair<ProgramEntityVisibility, ProgramEntityPtr> ProgramEntityVisibilityWithBinding;

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
 * I am the base interface for any program metamodel entity
 */
class ProgramEntity : public SubtypeOf<CompilerObject, ProgramEntity>
{
public:
    static constexpr char const __typeName__[] = "ProgramEntity";

    virtual bool isProgramEntity() const override;

    virtual ModulePtr getDefinitionModule() const;
    virtual AnyValuePtr getName() const;

    virtual void recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild);
    virtual void bindSymbolWithVisibility(const AnyValuePtr &symbol, ProgramEntityVisibility visibility, const ProgramEntityPtr &binding);
    virtual void bindProgramEntityWithVisibility(const ProgramEntityPtr &programEntity, ProgramEntityVisibility visibility);

    /// This method add a new macro method into the method dictionary with the specified selector.
    virtual void addMacroMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector);

    /// This method adds the method in the specified categories.
    virtual void addMacroMethodCategories(const MethodCategories &categories);

    /// This method add a new macro method into the method dictionary with the specified selector.
    virtual void addMacroFallbackMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector);

    /// This method adds the method in the specified categories.
    virtual void addMacroFallbackMethodCategories(const MethodCategories &categories);

    /// This method add a new method into the method dictionary with the specified selector.
    virtual void addMethodWithSelector(const AnyValuePtr &method, const AnyValuePtr &selector);

    /// This method adds the method in the specified categories.
    virtual void addMethodCategories(const MethodCategories &categories);
    
    virtual AnyValuePtr lookupExportedSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope);
    virtual AnyValuePtr lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope);

    /// Converts this program entity into a receiver type.
    virtual TypePtr asReceiverType();

    /// This method performs the lookup for a macro message or message with the specified selector and expansion level.
    virtual AnyValuePtr lookupSelectorInReceiverNodeWithExpansionLevel(const AnyValuePtr &selector, const ASTNodePtr &receiverNode, MessageSendExpansionLevel expansionLevel);

    /// This method performs the lookup for a macro message with the specified selector.
    virtual AnyValuePtr lookupMacroSelector(const AnyValuePtr &selector);

    /// This method performs the lookup of macro fallback message with the specified selector.
    virtual AnyValuePtr lookupMacroFallbackSelector(const AnyValuePtr &selector);

    /// This method performs the lookup for a message with the specified selector.
    virtual AnyValuePtr lookupSelector(const AnyValuePtr &selector);

    /// This method performs the lookup for a macro fallback message with the specified selector only in this type.
    virtual AnyValuePtr lookupLocalMacroFallbackSelector(const AnyValuePtr &selector);

    /// This method performs the lookup for a macro message with the specified selector only in this type.
    virtual AnyValuePtr lookupLocalMacroSelector(const AnyValuePtr &selector);

    /// This method performs the lookup for a message with the specified selector only in this type.
    virtual AnyValuePtr lookupLocalSelector(const AnyValuePtr &selector);

    /// Does this program entity support the definition of fields?
    virtual bool canHaveFields() const;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_HPP
