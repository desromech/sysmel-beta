#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_NAMESPACE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_NAMESPACE_HPP
#pragma once

#include "ModuleDefinedProgramEntity.hpp"
#include <unordered_map>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Namespace);

/**
 * I am a namespace.
 */
class Namespace : public SubtypeOf<ModuleDefinedProgramEntity, Namespace>
{
public:
    static constexpr char const __typeName__[] = "Namespace";

    static NamespacePtr makeWithName(const AnyValuePtr &name);

    virtual bool isNamespace() const override;

    virtual AnyValuePtr getName() const override;

    virtual void recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild);
    virtual void bindSymbolWithVisibility(const AnyValuePtr &symbol, ProgramEntityVisibility visibility, const ProgramEntityPtr &binding);
    virtual AnyValuePtr lookupExportedSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope) override;
    
protected:
    AnyValuePtr name;

    ProgramEntityPtrList children;
    std::unordered_map<AnyValuePtr, ProgramEntityVisibilityWithBinding> bindings;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_NAMESPACE_HPP