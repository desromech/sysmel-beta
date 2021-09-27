#include "sysmel/BootstrapEnvironment/Module.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
#include "sysmel/BootstrapEnvironment/Error.hpp"
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
    if(isLockedForNewDefinitions())
        throw UnsupportedOperation("Module is locked for new definitions.");

    registeredProgramEntities.insert(programEntity);
}

void Module::enqueueProgramEntitySemanticAnalysis(const ProgramEntityPtr &)
{
    throw UnsupportedOperation();
}

void Module::analyzeAllPendingProgramEntities(const ProgramEntityPtr &)
{
    // Nothing is required here by default.
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius