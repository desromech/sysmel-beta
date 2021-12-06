#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MODULE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MODULE_HPP
#pragma once

#include "ProgramEntity.hpp"
#include <unordered_set>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(Namespace);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ModuleDefinedProgramEntity);

/**
 * I am a particular module in the system.
 */
class Module : public SubtypeOf<ProgramEntity, Module>
{
public:
    static constexpr char const __typeName__[] = "Module";

    static ModulePtr getActive();
    static void setActive(const ModulePtr &aModule);

    template<typename FT>
    void activeDuring(const FT &f)
    {
        struct RestoreActive
        {
            RestoreActive()
            {
                oldActive = getActive();
            }

            ~RestoreActive()
            {
                setActive(oldActive);
            }

            ModulePtr oldActive;
        } restoreActive;

        setActive(selfFromThis());
        f();
    }
    
    virtual bool isBootstrapModule() const;
    virtual bool isProgramModule() const;
    virtual bool isScriptModule() const;

    bool isLockedForNewDefinitions();
    void lockForNewDefinitions();

    virtual void registerProgramEntity(const ProgramEntityPtr &programEntity);
    virtual void enqueueProgramEntitySemanticAnalysis(const ModuleDefinedProgramEntityPtr &programEntity);
    virtual void analyzeAllPendingProgramEntities();

    virtual NamespacePtr getGlobalNamespace() const;

protected:
    bool lockedForNewDefinitions = false;
    std::vector<ProgramEntityPtr> registeredProgramEntities;
    std::unordered_set<ProgramEntityPtr> registeredProgramEntitiesSet;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MODULE_HPP