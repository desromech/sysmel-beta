#include "sysmel/BootstrapEnvironment/Module.hpp"
#include "sysmel/BootstrapEnvironment/RuntimeContext.hpp"
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

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius