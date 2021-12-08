#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_SCOPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_SCOPE_HPP
#pragma once

#include "IdentifierLookupScope.hpp"
#include <unordered_map>

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ProgramEntityScope);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ProgramEntity);

/**
 * I am the interface for the scope that is used for the identifier lookup.
 */
class ProgramEntityScope : public SubtypeOf<IdentifierLookupScope, ProgramEntityScope>
{
public:
    static constexpr char const __typeName__[] = "ProgramEntityScope";

    static ProgramEntityScopePtr make(const IdentifierLookupScopePtr &parent, const ProgramEntityPtr &programEntity);

    virtual AnyValuePtr lookupSymbolLocally(const AnyValuePtr &symbol) override;

private:
    ProgramEntityPtr programEntity;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_ENTITY_SCOPE_HPP