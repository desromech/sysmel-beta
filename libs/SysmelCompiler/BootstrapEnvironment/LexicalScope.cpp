#include "sysmel/BootstrapEnvironment/LexicalScope.hpp"
#include "sysmel/BootstrapEnvironment/ProgramEntity.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<LexicalScope> LexicalScopeRegistration;

LexicalScopePtr LexicalScope::makeEmpty()
{
    return basicMakeObject<LexicalScope> ();
}

LexicalScopePtr LexicalScope::makeWithParent(const IdentifierLookupScopePtr &parent)
{
    auto result = basicMakeObject<LexicalScope> ();
    result->parent = parent;
    return result;
}

void LexicalScope::useNamespace(const ProgramEntityPtr &namespaceProgramEntity)
{
    usedNamespaces.push_back(namespaceProgramEntity);
}

bool LexicalScope::isLexicalScope() const
{
    return true;
}

AnyValuePtr LexicalScope::lookupSymbolLocally(const AnyValuePtr &symbol)
{
    auto it = boundSymbols.find(symbol);
    return it != boundSymbols.end() ? it->second : nullptr;
}

AnyValuePtr LexicalScope::lookupSymbolRecursively(const AnyValuePtr &symbol)
{
    {
        auto found = lookupSymbolLocally(symbol);
        if(found)
            return found;
    }

    for(const auto &nspace : usedNamespaces)
    {
        auto found = nspace->lookupExportedSymbolFromScope(symbol, selfFromThis());
        if(found)
            return found;
    }

    return parent ? parent->lookupSymbolRecursively(symbol) : nullptr;
}

void LexicalScope::setSymbolBinding(const AnyValuePtr &symbol, const AnyValuePtr &binding)
{
    boundSymbols[symbol] = binding;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius