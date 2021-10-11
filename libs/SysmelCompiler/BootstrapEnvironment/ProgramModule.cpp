#include "sysmel/BootstrapEnvironment/ProgramModule.hpp"
#include "sysmel/BootstrapEnvironment/Namespace.hpp"
#include "sysmel/BootstrapEnvironment/BootstrapTypeRegistration.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{
static BootstrapTypeRegistration<ProgramModule> programModuleTypeRegistration;

ProgramModulePtr ProgramModule::create(const std::string &name)
{
    auto result = std::make_shared<ProgramModule> ();
    result->name = name;
    result->activeDuring([&](){
        result->initialize();
    });
    
    return result;
}

void ProgramModule::initialize()
{
    SuperType::initialize();
    
    globalNamespace = Namespace::makeWithName(nullptr);
    globalNamespace->registerInCurrentModule();
}

NamespacePtr ProgramModule::getGlobalNamespace() const
{
    return globalNamespace;
}

bool ProgramModule::isProgramModule() const
{
    return true;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius