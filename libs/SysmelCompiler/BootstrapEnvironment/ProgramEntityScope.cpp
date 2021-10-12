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
    auto result = std::make_shared<ProgramEntityScope> ();
    result->parent = parent;
    result->programEntity = programEntity;
    return result;
}

AnyValuePtr ProgramEntityScope::lookupSymbolLocally(const AnyValuePtr &symbol)
{
    return programEntity->lookupLocalSymbolFromScope(symbol, shared_from_this());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius