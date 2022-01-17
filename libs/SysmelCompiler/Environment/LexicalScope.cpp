#include "Environment/LexicalScope.hpp"
#include "Environment/ProgramEntity.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<LexicalScope> LexicalScopeRegistration;

LexicalScopePtr LexicalScope::makeEmpty(const ASTSourcePositionPtr &sourcePosition)
{
    sysmelAssert(sourcePosition);
    auto result = basicMakeObject<LexicalScope> ();
    result->sourcePosition = sourcePosition;
    return result;
}

LexicalScopePtr LexicalScope::makeWithParent(const IdentifierLookupScopePtr &parent, const ASTSourcePositionPtr &sourcePosition)
{
    sysmelAssert(sourcePosition);
    auto result = basicMakeObject<LexicalScope> ();
    result->parent = parent;
    result->sourcePosition = sourcePosition;
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

ASTSourcePositionPtr LexicalScope::getSourcePosition() const
{
    return sourcePosition;
}

} // End of namespace Environment
} // End of namespace Sysmel