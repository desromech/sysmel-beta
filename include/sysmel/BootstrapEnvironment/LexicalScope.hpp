#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LEXICAL_SCOPE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_LEXICAL_SCOPE_HPP
#pragma once

#include "IdentifierLookupScope.hpp"
#include <unordered_map>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(LexicalScope);

/**
 * I am the interface for the scope that is used for the identifier lookup.
 */
class LexicalScope : public SubtypeOf<IdentifierLookupScope, LexicalScope>
{
public:
    static constexpr char const __typeName__[] = "LexicalScope";

    virtual bool isLexicalScope() const override;

    /// This method performs a symbol lookup locally.
    virtual AnyValuePtr lookupSymbolLocally(const AnyValuePtr &symbol);

private:
    std::unordered_map<AnyValuePtr, AnyValuePtr> boundSymbols;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_IDENTIFIER_LOOKUP_SCOPE_HPP