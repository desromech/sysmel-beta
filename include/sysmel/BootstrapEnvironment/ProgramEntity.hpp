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
    virtual void bindProgramEntityWithVisibility(ProgramEntityVisibility visibility, const ProgramEntityPtr &binding);

    virtual AnyValuePtr lookupExportedSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope);
    virtual AnyValuePtr lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope);
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_HPP
