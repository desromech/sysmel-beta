#include "sysmel/BootstrapEnvironment/LexicalScope.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<LexicalScope> LexicalScopeRegistration;

bool LexicalScope::isLexicalScope() const
{
    return true;
}

AnyValuePtr LexicalScope::lookupSymbolLocally(const AnyValuePtr &symbol)
{
    auto it = boundSymbols.find(symbol);
    return it != boundSymbols.end() ? it->second : nullptr;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius