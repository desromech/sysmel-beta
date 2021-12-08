#ifndef SYSMEL_ENVIRONMENT_IDENTIFIER_LOOKUP_SCOPE_HPP
#define SYSMEL_ENVIRONMENT_IDENTIFIER_LOOKUP_SCOPE_HPP
#pragma once

#include "CompilerObject.hpp"

namespace Sysmel
{
namespace Environment
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

    /// Is the name reserved in this specific scope so that it cannot be overriden?
    virtual bool isNameReservedLocally(const AnyValuePtr &symbol);

    /// Is the name reserved so that it cannot be overriden?
    virtual bool isNameReserved(const AnyValuePtr &symbol);

    IdentifierLookupScopePtr parent;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_IDENTIFIER_LOOKUP_SCOPE_HPP