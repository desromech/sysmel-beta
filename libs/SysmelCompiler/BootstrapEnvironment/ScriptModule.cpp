#include "sysmel/BootstrapEnvironment/ScriptModule.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<ScriptModule> scriptModuleTypeRegistration;

ScriptModulePtr ScriptModule::create()
{
    auto result = basicMakeObject<ScriptModule> ();
    result->activeDuring([&](){
        result->initialize();
    });

    return result;
}

bool ScriptModule::isScriptModule() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius