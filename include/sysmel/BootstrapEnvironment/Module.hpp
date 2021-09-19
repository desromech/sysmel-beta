#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MODULE_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MODULE_HPP
#pragma once

#include "ProgramEntity.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

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

        setActive(shared_from_this());
        f();
    }
    
    virtual bool isBootstrapModule() const;
    virtual bool isTranslationUnitModule() const;

};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_MODULE_HPP