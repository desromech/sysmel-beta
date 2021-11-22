#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RUNTIME_CONTEXT_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RUNTIME_CONTEXT_HPP
#pragma once

#include "ProgramEntity.hpp"
#include "Exception.hpp"
#include <map>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(BootstrapModule);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LanguageSupport);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(PointerType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ReferenceType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(TemporaryReferenceType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(TupleType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(MethodType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClosureType);

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

        // Make sure to invoke what on the exception so that it can be readed outside this context.
        try
        {
            f();
        }
        catch(const ExceptionWrapper& e)
        {
            e.what();
            throw e;
        }
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
    friend class FunctionType;
    friend class ClosureType;
    friend class MethodType;
    friend class PointerType;
    friend class ReferenceType;
    friend class TemporaryReferenceType;
    
    BootstrapModulePtr bootstrapModule;
    LanguageSupportPtr sysmelLanguageSupport;

    std::map<std::pair<TypePtr, AnyValuePtr>, PointerTypePtr> pointerTypeCache;
    std::map<std::pair<TypePtr, AnyValuePtr>, ReferenceTypePtr> referenceTypeCache;
    std::map<std::pair<TypePtr, AnyValuePtr>, TemporaryReferenceTypePtr> temporaryReferenceTypeCache;
    std::map<std::vector<TypePtr>, TupleTypePtr> tupleTypeCache;
};

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RUNTIME_CONTEXT_HPP