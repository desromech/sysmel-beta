#include "sysmel/BootstrapEnvironment/Namespace.hpp"
#include "sysmel/BootstrapEnvironment/Type.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<Namespace> namespaceTypeRegistration;

TypePtr Type::getNamespaceType()
{
    return Namespace::__staticType__();
}

NamespacePtr Namespace::makeWithName(const AnyValuePtr &name)
{
    auto result = std::make_shared<Namespace> ();
    result->name = name;
    return result;
}

bool Namespace::isNamespace() const
{
    return true;
}

AnyValuePtr Namespace::getName() const
{
    return name;
}

void Namespace::recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild)
{
    children.push_back(newChild);
}

void Namespace::bindSymbolWithVisibility(const AnyValuePtr &symbol, ProgramEntityVisibility visibility, const ProgramEntityPtr &binding)
{
    assert(symbol && !symbol->isAnonymousNameSymbol());
    if(bindings.find(symbol) != bindings.end())
        signalNewWithMessage<Error> ("Expected a new symbol binding.");

    bindings[symbol] = std::make_pair(visibility, binding);
}

AnyValuePtr Namespace::lookupExportedSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    auto it = bindings.find(symbol);
    if(it == bindings.end())
        return nullptr;

    auto &binding = it->second;
    switch(binding.first)
    {
    case ProgramEntityVisibility::Public:
        return binding.second;
    default:
        return nullptr;
    }
}

AnyValuePtr Namespace::lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    auto it = bindings.find(symbol);
    return it != bindings.end() ? it->second.second : nullptr;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius