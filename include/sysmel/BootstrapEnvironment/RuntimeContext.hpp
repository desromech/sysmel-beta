#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RUNTIME_CONTEXT_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RUNTIME_CONTEXT_HPP
#pragma once

#include "ProgramEntity.hpp"

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(BootstrapModule);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LanguageSupport);

/**
 * I represent an active runtime context in the object model environment.
 */
class RuntimeContext : public SubtypeOf<ProgramEntity, RuntimeContext>
{
public:
    static constexpr char const __typeName__[] = "RuntimeContext";

    static RuntimeContextPtr create();

    static RuntimeContextPtr getActive();
    static void setActive(const RuntimeContextPtr &aRuntimeContext);

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

            RuntimeContextPtr oldActive;
        } restoreActive;

        setActive(shared_from_this());
        f();
    }

    virtual void initialize() override;

    const BootstrapModulePtr &getBootstrapModule()
    {
        return bootstrapModule;
    }

    const LanguageSupportPtr &getSysmelLanguageSupport()
    {
        return sysmelLanguageSupport;
    }

protected:
    
    BootstrapModulePtr bootstrapModule;
    LanguageSupportPtr sysmelLanguageSupport;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RUNTIME_CONTEXT_HPP