#include "Environment/Module.hpp"
#include "Environment/RuntimeContext.hpp"
#include "Environment/UnsupportedOperation.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
{
static BootstrapTypeRegistration<Module> moduleTypeRegistration;

static thread_local ModulePtr activeModuleInThisThread;

ModulePtr Module::getActive()
{
    return activeModuleInThisThread;
}

void Module::setActive(const ModulePtr &aModule)
{
    activeModuleInThisThread = aModule;
}

bool Module::isBootstrapModule() const
{
    return false;
}

bool Module::isProgramModule() const
{
    return false;
}

bool Module::isScriptModule() const
{
    return false;
}

bool Module::isLockedForNewDefinitions()
{
    return lockedForNewDefinitions;
}

void Module::lockForNewDefinitions()
{
    lockedForNewDefinitions = true;
}

void Module::registerProgramEntity(const ProgramEntityPtr &programEntity)
{
    if(registeredProgramEntitiesSet.find(programEntity) != registeredProgramEntitiesSet.end())
        return;
        
    if(isLockedForNewDefinitions())
        signalNewWithMessage<UnsupportedOperation> ("Module is locked for new definitions.");

    registeredProgramEntitiesSet.insert(programEntity);
    registeredProgramEntities.push_back(programEntity);
}

void Module::enqueueProgramEntitySemanticAnalysis(const ModuleDefinedProgramEntityPtr &)
{
    signalNew<UnsupportedOperation> ();
}

void Module::analyzeAllPendingProgramEntities()
{
    // Nothing is required here by default.
}

NamespacePtr Module::getGlobalNamespace() const
{
    return nullptr;
}

ModulePtr Module::getDefinitionModule()
{
    return selfFromThis();
}

} // End of namespace Environment
} // End of namespace Sysmel