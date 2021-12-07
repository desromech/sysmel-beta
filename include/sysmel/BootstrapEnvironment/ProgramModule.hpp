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
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAModule);

/**
 * I am a particular module in the system.
 */
class ProgramModule : public SubtypeOf<Module, ProgramModule>
{
public:
    static constexpr char const __typeName__[] = "ProgramModule";

    static ProgramModulePtr create(const std::string &name);
    virtual void initialize() override;

    virtual NamespacePtr getGlobalNamespace() const override;

    virtual bool isProgramModule() const override;

    virtual void enqueueProgramEntitySemanticAnalysis(const ModuleDefinedProgramEntityPtr &programEntity) override;
    virtual void analyzeAllPendingProgramEntities() override;

    virtual SExpression asSExpression() const override;
    virtual SExpression asFullDefinitionSExpression() const override;
    virtual SSAValuePtr asSSAValueRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition) override;

protected:
    std::string name;
    NamespacePtr globalNamespace;
    SSAModulePtr ssaModule;

    std::vector<ModuleReferencePtr> importedModules;
    std::vector<ModuleReferencePtr> indirectlyImportedModules;
    std::vector<ModuleDefinedProgramEntityPtr> programEntitiesWithPendingAnalysis;
    std::unordered_set<ModuleDefinedProgramEntityPtr> programEntitiesWithPendingAnalysisSet;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_PROGRAM_MODULE_HPP