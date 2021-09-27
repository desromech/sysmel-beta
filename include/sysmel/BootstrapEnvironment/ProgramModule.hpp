#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_MODULE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_MODULE_HPP
#pragma once

#include "Module.hpp"
#include "ModuleReference.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(ProgramModule);

/**
 * I am a particular module in the system.
 */
class ProgramModule : public SubtypeOf<Module, ProgramModule>
{
public:
    static constexpr char const __typeName__[] = "ProgramModule";

    static ProgramModulePtr create(const std::string &name);

    virtual bool isProgramModule() const override;

protected:
    std::string name;

    std::vector<ModuleReferencePtr> importedModules;
    std::vector<ModuleReferencePtr> indirectlyImportedModules;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_MODULE_HPP