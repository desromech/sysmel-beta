#ifndef SYSMEL_ENVIRONMENT_MODULE_DEFINED_PROGRAM_ENTITY
#define SYSMEL_ENVIRONMENT_MODULE_DEFINED_PROGRAM_ENTITY
#pragma once

#include "ProgramEntity.hpp"
#include "ProgramEntityVisibility.hpp"
#include "DllLinkageMode.hpp"
#include "ExternalLanguageMode.hpp"


namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ModuleDefinedProgramEntity);

/**
 * I am the base interface for most of the methods that are defined in the system.
 */
class ModuleDefinedProgramEntity : public SubtypeOf<ProgramEntity, ModuleDefinedProgramEntity>
{
public:
    static constexpr char const __typeName__[] = "ModuleDefinedProgramEntity";

    virtual bool isSemanticAnalyzed() const;

    virtual void ensureSemanticAnalysis();
    virtual void enqueuePendingSemanticAnalysis();

    virtual ModulePtr getDefinitionModule() override;

    ProgramEntityVisibility getVisiblity() const;
    void setVisibility(ProgramEntityVisibility newVisibility);

    DllLinkageMode getDllLinkageMode() const;
    void setDllLinkageMode(DllLinkageMode newDllLinkageMode);

    ExternalLanguageMode getExternalLanguageMode() const;
    void setExternalLanguageMode(ExternalLanguageMode newExternalLanguageMode);
protected:
    ProgramEntityVisibility visibility = ProgramEntityVisibility::Default;
    DllLinkageMode dllLinkageMode = DllLinkageMode::None;
    ExternalLanguageMode externalLanguageMode = ExternalLanguageMode::None;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_MODULE_DEFINED_PROGRAM_ENTITY