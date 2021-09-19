#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_IDENTIFIER_LOOKUP_SCOPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_IDENTIFIER_LOOKUP_SCOPE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(IdentifierLookupScope);

/**
 * I am the interface for the scope that is used for the identifier lookup.
 */
class IdentifierLookupScope : public SubtypeOf<CompilerObject, IdentifierLookupScope>
{
public:
    static constexpr char const __typeName__[] = "IdentifierLookupScope";

    virtual bool isIdentifierLookupScope() const override;

    /// Is this a local lexical scope?
    virtual bool isLexicalScope() const;

    /// This method performs a symbol lookup locally.
    virtual AnyValuePtr lookupSymbolLocally(const AnyValuePtr &symbol);

    /// This method performs a recursive symbol lookup.
    virtual AnyValuePtr lookupSymbolRecursively(const AnyValuePtr &symbol);

    IdentifierLookupScopePtr parent;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_IDENTIFIER_LOOKUP_SCOPE_HPP