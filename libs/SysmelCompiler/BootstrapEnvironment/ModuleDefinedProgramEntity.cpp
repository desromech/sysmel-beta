#include "sysmel/BootstrapEnvironment/ModuleDefinedProgramEntity.hpp"
#include "sysmel/BootstrapEnvironment/Module.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<ModuleDefinedProgramEntity> moduleTypeRegistration;

bool ModuleDefinedProgramEntity::isSemanticAnalyzed() const
{
    return false;
}

ModulePtr ModuleDefinedProgramEntity::getDefinitionModule() const
{
    return definitionModule.lock();
}

void ModuleDefinedProgramEntity::registerInCurrentModule()
{
    auto existentDefinition = definitionModule.lock();
    if(existentDefinition)
        signalNewWithMessage<Error> ("Module defined program entities can only be registered once.");
    
    auto currentModule = Module::getActive();
    if(!currentModule)
        signalNewWithMessage<Error> ("An active module is required here.");

    currentModule->registerProgramEntity(shared_from_this());
    definitionModule = currentModule;
}

void ModuleDefinedProgramEntity::ensureSemanticAnalysis()
{
}

void ModuleDefinedProgramEntity::enqueuePendingSemanticAnalysis()
{
    getDefinitionModule()->enqueueProgramEntitySemanticAnalysis(shared_from_this());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius