#include "Environment/IdentifierLookupScope.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<IdentifierLookupScope> IdentifierLookupScopeRegistration;

bool IdentifierLookupScope::isIdentifierLookupScope() const
{
    return true;
}

bool IdentifierLookupScope::isLexicalScope() const
{
    return false;
}

AnyValuePtr IdentifierLookupScope::lookupSymbolLocally(const AnyValuePtr &)
{
    return nullptr;
}

AnyValuePtr IdentifierLookupScope::lookupSymbolRecursively(const AnyValuePtr &symbol)
{
    auto found = lookupSymbolLocally(symbol);
    if(found)
        return found;
    return parent ? parent->lookupSymbolRecursively(symbol) : nullptr;
}

bool IdentifierLookupScope::isNameReservedLocally(const AnyValuePtr &)
{
    return false;
}

bool IdentifierLookupScope::isNameReserved(const AnyValuePtr &symbol)
{
    if(isNameReservedLocally(symbol))
        return true;

    return parent ? parent->isNameReserved(symbol) : false;
}

} // End of namespace Environment
} // End of namespace Sysmel