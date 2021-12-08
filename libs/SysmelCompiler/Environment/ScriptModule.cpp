#include "Environment/ScriptModule.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel