#include "sysmel/BootstrapEnvironment/ProgramEntity.hpp"
#include "sysmel/BootstrapEnvironment/UnsupportedOperation.hpp"
#include "sysmel/BootstrapEnvironment/Module.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"
#include "sysmel/BootstrapEnvironment/StringUtilities.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

static BootstrapTypeRegistration<ProgramEntity> programEntityTypeRegistration;

bool ProgramEntity::isProgramEntity() const
{
    return true;
}

ModulePtr ProgramEntity::getDefinitionModule() const
{
    return ModulePtr();
}

AnyValuePtr ProgramEntity::getName() const
{
    return nullptr;
}

void ProgramEntity::recordChildProgramEntityDefinition(const ProgramEntityPtr &newChild)
{
    signalNewWithMessage<UnsupportedOperation> (formatString("Cannot record child program {0} entity inside of {1}.",
        {{newChild->printString(), printString()}}));
}

void ProgramEntity::bindSymbolWithVisibility(const AnyValuePtr &symbol, ProgramEntityVisibility visibility, const ProgramEntityPtr &binding)
{
    (void)visibility;
    signalNewWithMessage<UnsupportedOperation> (formatString("Cannot bind child program {0} with symbol {1} entity inside of {2}.",
        {{binding->printString(), validAnyValue(symbol)->printString(), printString()}}));
}

void ProgramEntity::bindProgramEntityWithVisibility(ProgramEntityVisibility visibility, const ProgramEntityPtr &binding)
{
    bindSymbolWithVisibility(binding->getName(), visibility, binding);
}

AnyValuePtr ProgramEntity::lookupExportedSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    (void)symbol;
    (void)accessingScope;
    return nullptr;
}

AnyValuePtr ProgramEntity::lookupLocalSymbolFromScope(const AnyValuePtr &symbol, const IdentifierLookupScopePtr &accessingScope)
{
    (void)symbol;
    (void)accessingScope;
    return nullptr;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius
