#include "sysmel/BootstrapEnvironment/Module.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/UnsupportedOperation.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius