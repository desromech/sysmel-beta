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

ModulePtr ModuleDefinedProgramEntity::getDefinitionModule()
{
    auto parent = getParentProgramEntity();
    return parent ? parent->getDefinitionModule() : nullptr;
}

void ModuleDefinedProgramEntity::ensureSemanticAnalysis()
{
}

void ModuleDefinedProgramEntity::enqueuePendingSemanticAnalysis()
{
    Module::getActive()->enqueueProgramEntitySemanticAnalysis(selfFromThis());
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius