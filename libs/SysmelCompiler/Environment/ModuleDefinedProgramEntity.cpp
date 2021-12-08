#include "Environment/ModuleDefinedProgramEntity.hpp"
#include "Environment/Module.hpp"
#include "Environment/Error.hpp"
#include "Environment/BootstrapTypeRegistration.hpp"

namespace Sysmel
{
namespace Environment
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

} // End of namespace Environment
} // End of namespace Sysmel