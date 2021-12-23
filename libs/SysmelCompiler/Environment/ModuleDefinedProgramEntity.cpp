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

ProgramEntityVisibility ModuleDefinedProgramEntity::getVisiblity() const
{
    return visibility;
}

void ModuleDefinedProgramEntity::setVisibility(ProgramEntityVisibility newVisibility)
{
    visibility = newVisibility;
}

DllLinkageMode ModuleDefinedProgramEntity::getDllLinkageMode() const
{
    return dllLinkageMode;
}

void ModuleDefinedProgramEntity::setDllLinkageMode(DllLinkageMode newDllLinkageMode)
{
    dllLinkageMode = newDllLinkageMode;
}

ExternalLanguageMode ModuleDefinedProgramEntity::getExternalLanguageMode() const
{
    return externalLanguageMode;
}

void ModuleDefinedProgramEntity::setExternalLanguageMode(ExternalLanguageMode newExternalLanguageMode)
{
    externalLanguageMode = newExternalLanguageMode;
}

} // End of namespace Environment
} // End of namespace Sysmel