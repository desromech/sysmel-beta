#ifndef SYSMEL_ENVIRONMENT_MODULE_DEFINED_PROGRAM_ENTITY
#define SYSMEL_ENVIRONMENT_MODULE_DEFINED_PROGRAM_ENTITY
#pragma once

#include "ProgramEntity.hpp"

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
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_MODULE_DEFINED_PROGRAM_ENTITY