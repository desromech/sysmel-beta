#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RUNTIME_CONTEXT_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RUNTIME_CONTEXT_HPP
#pragma once

#include "RuntimeContextTargetDescription.hpp"
#include "ProgramEntity.hpp"
#include "Exception.hpp"
#include <map>
#include <unordered_map>
#include <tuple>

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(BootstrapModule);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LanguageSupport);

SYSMEL_DECLARE_BOOTSTRAP_CLASS(DecoratedType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(PointerType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ReferenceType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(TemporaryReferenceType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(TupleType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(VariantType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ArrayType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(FunctionType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(MethodType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(ClosureType);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(LiteralSymbol);

/**
 * I represent an active runtime context in the object model environment.
 */
class RuntimeContext : public SubtypeOf<ProgramEntity, RuntimeContext>
{
public:
    static constexpr char const __typeName__[] = "RuntimeContext";

    static RuntimeContextPtr createForTarget(const RuntimeContextTargetDescription &target);
    static RuntimeContextPtr createForScripting();

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

        setActive(selfFromThis());

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

    RefCountedObjectMemoryPool *getContextMemoryPool()
    {
        return &memoryPool;
    }

    const RuntimeContextTargetDescription &getTargetDescription()
    {
        return target;
    }

protected:
    friend class FunctionType;
    friend class ClosureType;
    friend class MethodType;
    friend class DecoratedType;
    friend class PointerType;
    friend class ReferenceType;
    friend class TemporaryReferenceType;
    friend class ArrayType;
    friend class TupleType;
    friend class VariantType;
    friend class LiteralSymbol;
    
    RefCountedObjectMemoryPool memoryPool;
    RuntimeContextTargetDescription target;

    BootstrapModulePtr bootstrapModule;
    LanguageSupportPtr sysmelLanguageSupport;

    std::map<std::pair<TypePtr, uint32_t>, DecoratedTypePtr> decoratedTypeCache;
    std::map<std::pair<TypePtr, AnyValuePtr>, PointerTypePtr> pointerTypeCache;
    std::map<std::pair<TypePtr, AnyValuePtr>, ReferenceTypePtr> referenceTypeCache;
    std::map<std::pair<TypePtr, AnyValuePtr>, TemporaryReferenceTypePtr> temporaryReferenceTypeCache;
    std::map<std::pair<TypePtr, uint64_t>, ArrayTypePtr> arrayTypeCache;
    std::map<TypePtrList, TupleTypePtr> tupleTypeCache;
    std::map<TypePtrList, VariantTypePtr> variantTypeCache;

    std::map<std::pair<TypePtr, TypePtrList>, FunctionTypePtr> functionTypeCache;
    std::map<std::pair<TypePtr, TypePtrList>, ClosureTypePtr> closureTypeCache;
    std::map<std::tuple<TypePtr, TypePtr, TypePtrList>, MethodTypePtr> methodTypeCache;

    std::unordered_map<std::string, LiteralSymbolPtr> symbolInternTable;
};

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_RUNTIME_CONTEXT_HPP