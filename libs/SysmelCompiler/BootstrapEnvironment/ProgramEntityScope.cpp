#include "sysmel/BootstrapEnvironment/ProgramEntityScope.hpp"
#include "sysmel/BootstrapEnvironment/ProgramEntity.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ProgramEntityScope> ProgramEntityScopeRegistration;

ProgramEntityScopePtr ProgramEntityScope::make(const IdentifierLookupScopePtr &parent, const ProgramEntityPtr &programEntity)
{
    auto result = basicMakeObject<ProgramEntityScope> ();
    result->parent = parent;
    result->programEntity = programEntity;
    return result;
}

AnyValuePtr ProgramEntityScope::lookupSymbolLocally(const AnyValuePtr &symbol)
{
    return programEntity->lookupLocalSymbolFromScope(symbol, selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius