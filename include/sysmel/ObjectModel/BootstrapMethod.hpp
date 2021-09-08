#ifndef SYSMEL_COMPILER_OBJECT_MODEL_BOOTSTRAP_METHOD_HPP
#define SYSMEL_COMPILER_OBJECT_MODEL_BOOTSTRAP_METHOD_HPP
#pragma once

#include "SpecificMethod.hpp"
#include "Wrappers.hpp"
#include "Error.hpp"
#include <type_traits>

namespace SysmelMoebius
{
namespace ObjectModel
{

/**
 * I am the base interface for a type that is specifically defined by the bootstrap environment.
 */
class BootstrapMethodBase : public SubtypeOf<SpecificMethod, BootstrapMethodBase>
{
public:
    static constexpr char const __typeName__[] = "BootstrapMethod";
    
    BootstrapMethodBase(const AnyValuePtr &initialSelector, size_t initialArgumentCount)
        : selector(initialSelector), argumentCount(initialArgumentCount)
    {
    }

    virtual bool isBootstrapMethod() const override;

protected:
    AnyValuePtr selector;
    size_t argumentCount;
};

template<typename MethodSignature, typename FT>
class BootstrapMethod;

template<typename ResultType, typename ReceiverType, typename... Args, typename FT>
class BootstrapMethod<ResultType (ReceiverType, Args...), FT> : public BootstrapMethodBase
{
public:
    typedef BootstrapMethod<ResultType (Args...), FT> SelfType;

    BootstrapMethod(const AnyValuePtr &initialSelector, FT initialFunctor)
        : BootstrapMethodBase(initialSelector, sizeof...(Args)), functor(initialFunctor)
    {
        selector = initialSelector;
        signature = MethodSignature{
            wrapperTypeFor<ResultType> (),
            wrapperTypeFor<ReceiverType> (),
            {
                wrapperTypeFor<Args> ()...
            }
        };
    }

    AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver) override
    {
        (void)selector;
        
        if(argumentCount != arguments.size())
            throw ArgumentsCountMismatch(argumentCount, arguments.size());

        size_t index = 0;
        if constexpr(std::is_same<ResultType, void>::value)
        {
            functor(unwrapValue<ReceiverType> (receiver), unwrapValue<Args> (arguments[index++])...);
            return getVoidConstant();
        }
        else
        {
            return wrapValue(functor(unwrapValue<ReceiverType> (receiver), unwrapValue<Args> (arguments[index++])...));
        }
    }
private:
    FT functor;
};


template<typename MethodSignature, typename FT>
MethodBinding makeMethodBinding(const std::string &selector, FT &&functor)
{
    auto selectorSymbol = internSymbol(selector);
    auto bootstrapMethod = std::make_shared<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    return MethodBinding{selectorSymbol, bootstrapMethod};
}

} // End of namespace ObjectModel
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_OBJECT_MODEL_BOOTSTRAP_TYPE_HPP