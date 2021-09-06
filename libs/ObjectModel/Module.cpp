#include "sysmel/ObjectModel/Module.hpp"
#include "sysmel/ObjectModel/RuntimeContext.hpp"
#include "sysmel/ObjectModel/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace ObjectModel
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

bool Module::isTranslationUnitModule() const
{
    return false;
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius