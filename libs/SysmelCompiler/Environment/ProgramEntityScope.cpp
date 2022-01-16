#include "Environment/ProgramEntityScope.hpp"
#include "Environment/ProgramEntity.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{

static BootstrapTypeRegistration<ProgramEntityScope> ProgramEntityScopeRegistration;

ProgramEntityScopePtr ProgramEntityScope::make(const IdentifierLookupScopePtr &parent, const ProgramEntityPtr &programEntity)
{
    auto result = basicMakeObject<ProgramEntityScope> ();
    result->parent = parent;
    result->programEntity = programEntity;
    return result;
}

bool ProgramEntityScope::isProgramEntityScope() const
{
    return true;
}

AnyValuePtr ProgramEntityScope::lookupSymbolLocally(const AnyValuePtr &symbol)
{
    return programEntity->lookupLocalSymbolFromScope(symbol, selfFromThis());
}

} // End of namespace Environment
} // End of namespace Sysmel