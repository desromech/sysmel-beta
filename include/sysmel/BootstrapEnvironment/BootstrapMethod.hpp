#ifndef SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_METHOD_HPP
#define SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_METHOD_HPP
#pragma once

#include "SpecificMethod.hpp"
#include "Wrappers.hpp"
#include "ArgumentCountError.hpp"
#include "FunctionalType.hpp"
#include <type_traits>

namespace SysmelMoebius
{
namespace BootstrapEnvironment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(BootstrapMethodBase);

/**
 * I am the base interface for a type that is specifically defined by the bootstrap environment.
 */
class BootstrapMethodBase : public SubtypeOf<SpecificMethod, BootstrapMethodBase>
{
public:
    static constexpr char const __typeName__[] = "BootstrapMethod";
    
    virtual bool isBootstrapMethod() const override;
    virtual bool isCompileTimeEvaluableMethod() const override;
};

template<typename MethodSignature, typename FT = void>
class BootstrapMethod;

template<typename ResultType, typename ReceiverType, typename... Args, typename FT>
class BootstrapMethod<ResultType (ReceiverType, Args...), FT> : public BootstrapMethodBase
{
public:
    typedef BootstrapMethod<ResultType (Args...), FT> SelfType;

    BootstrapMethod(const AnyValuePtr &initialSelector, FT initialFunctor)
        : functor(initialFunctor)
    {
        setName(initialSelector);
        setMethodSignature(wrapperTypeFor<ReceiverType> (), wrapperTypeForReturning<ResultType> (), { wrapperTypeFor<Args> ()...});
    }

    AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver) override
    {
        (void)selector;
        
        if(functionalType->getArgumentCount() != arguments.size())
        {
            auto error = std::make_shared<ArgumentCountError> ();
            error->expectedCount = functionalType->getArgumentCount();
            error->callCount = arguments.size();
            error->signal();
        }

        return doRunWithArgumentsIn(std::make_index_sequence<sizeof...(Args)> {}, arguments, receiver);
    }
private:

    template<typename T, T... ArgIndices>
    AnyValuePtr doRunWithArgumentsIn(std::integer_sequence<T, ArgIndices...> indicesSeq, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
    {
        (void)indicesSeq;

        if constexpr(std::is_same<ResultType, void>::value)
        {
            functor(unwrapValue<ReceiverType> (receiver), unwrapValue<Args> (arguments[ArgIndices])...);
            return getVoidConstant();
        }
        else
        {
            return wrapValue(functor(unwrapValue<ReceiverType> (receiver), unwrapValue<Args> (arguments[ArgIndices])...));
        }
    }

    FT functor;
};


template<typename ResultType, typename ReceiverType, typename... Args>
class BootstrapMethod<ResultType (ReceiverType::*) (Args...)> : public BootstrapMethodBase
{
public:
    typedef BootstrapMethod<ResultType (ReceiverType::*) (Args...)> SelfType;
    typedef ResultType (ReceiverType::*MemberFunctionPointerType) (Args...);

    BootstrapMethod(const AnyValuePtr &initialSelector, MemberFunctionPointerType initialMemberFunctionPointer)
        : memberFunctionPointer(initialMemberFunctionPointer)
    {
        setName(initialSelector);
        setMethodSignature(wrapperTypeFor<ReceiverType*> (), wrapperTypeForReturning<ResultType> (), { wrapperTypeFor<Args> ()...});
    }

    AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver) override
    {
        (void)selector;
        
        if(functionalType->getArgumentCount() != arguments.size())
        {
            auto error = std::make_shared<ArgumentCountError> ();
            error->expectedCount = functionalType->getArgumentCount();
            error->callCount = arguments.size();
            error->signal();
        }

        return doRunWithArgumentsIn(std::make_index_sequence<sizeof...(Args)> {}, arguments, receiver);
    }
private:
    template<typename T, T... ArgIndices>
    AnyValuePtr doRunWithArgumentsIn(std::integer_sequence<T, ArgIndices...> indicesSeq, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
    {
        (void)indicesSeq;

        auto unwrappedReceiver = unwrapValue<ReceiverType*> (receiver);
        if constexpr(std::is_same<ResultType, void>::value)
        {
            (unwrappedReceiver->*memberFunctionPointer)(unwrapValue<Args> (arguments[ArgIndices])...);
            return getVoidConstant();
        }
        else
        {
            return wrapValue((unwrappedReceiver->*memberFunctionPointer)(unwrapValue<Args> (arguments[ArgIndices])...));
        }
    }

    MemberFunctionPointerType memberFunctionPointer;
};

template<typename ResultType, typename ReceiverType, typename... Args>
class BootstrapMethod<ResultType (ReceiverType::*) (Args...) const> : public BootstrapMethodBase
{
public:
    typedef BootstrapMethod<ResultType (ReceiverType::*) (Args...)> SelfType;
    typedef ResultType (ReceiverType::*MemberFunctionPointerType) (Args...) const;

    BootstrapMethod(const AnyValuePtr &initialSelector, MemberFunctionPointerType initialMemberFunctionPointer)
        : memberFunctionPointer(initialMemberFunctionPointer)
    {
        setName(initialSelector);
        setMethodSignature(wrapperTypeFor<const ReceiverType*> (), wrapperTypeForReturning<ResultType> (), { wrapperTypeFor<Args> ()...});
    }

    AnyValuePtr runWithArgumentsIn(const AnyValuePtr &selector, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver) override
    {
        (void)selector;
        
        if(functionalType->getArgumentCount() != arguments.size())
        {
            auto error = std::make_shared<ArgumentCountError> ();
            error->expectedCount = functionalType->getArgumentCount();
            error->callCount = arguments.size();
            error->signal();
        }

        return doRunWithArgumentsIn(std::make_index_sequence<sizeof...(Args)> {}, arguments, receiver);
    }

private:
    template<typename T, T... ArgIndices>
    AnyValuePtr doRunWithArgumentsIn(std::integer_sequence<T, ArgIndices...> indicesSeq, const std::vector<AnyValuePtr> &arguments, const AnyValuePtr &receiver)
    {
        (void)indicesSeq;

        auto unwrappedReceiver = unwrapValue<ReceiverType*> (receiver);
        if constexpr(std::is_same<ResultType, void>::value)
        {
            (unwrappedReceiver->*memberFunctionPointer)(unwrapValue<Args> (arguments[ArgIndices])...);
            return getVoidConstant();
        }
        else
        {
            return wrapValue((unwrappedReceiver->*memberFunctionPointer)(unwrapValue<Args> (arguments[ArgIndices])...));
        }
    }

    MemberFunctionPointerType memberFunctionPointer;
};


template<typename MethodSignature, typename FT>
BootstrapMethodBasePtr makeBootstrapMethod(const std::string &selector, FT &&functor)
{
    auto selectorSymbol = internSymbol(selector);
    return std::make_shared<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
}

template<typename MethodSignature, typename FT>
MethodBinding makeMethodBinding(const std::string &selector, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto selectorSymbol = internSymbol(selector);
    auto bootstrapMethod = std::make_shared<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->addMethodFlags(flags);
    bootstrapMethod->registerInCurrentModule();
    return MethodBinding{selectorSymbol, bootstrapMethod};
}

template<typename FT>
MethodBinding makeMethodBinding(const std::string &selector, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto selectorSymbol = internSymbol(selector);
    auto bootstrapMethod = std::make_shared<BootstrapMethod<FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->addMethodFlags(flags);
    bootstrapMethod->registerInCurrentModule();
    return MethodBinding{selectorSymbol, bootstrapMethod};
}

template<typename MethodSignature, typename FT>
MethodBinding makeIntrinsicMethodBinding(const std::string &intrinsicName, const std::string &selector, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto intrinsicSymbol = internSymbol(intrinsicName);
    auto selectorSymbol = internSymbol(selector);
    auto bootstrapMethod = std::make_shared<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->setIntrinsicName(intrinsicSymbol);
    bootstrapMethod->addMethodFlags(flags);
    bootstrapMethod->registerInCurrentModule();
    return MethodBinding{selectorSymbol, bootstrapMethod};
}

template<typename FT>
MethodBinding makeIntrinsicMethodBinding(const std::string &intrinsicName, const std::string &selector, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto intrinsicSymbol = internSymbol(intrinsicName);
    auto selectorSymbol = internSymbol(selector);
    auto bootstrapMethod = std::make_shared<BootstrapMethod<FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->setIntrinsicName(intrinsicSymbol);
    bootstrapMethod->addMethodFlags(flags);
    bootstrapMethod->registerInCurrentModule();
    return MethodBinding{selectorSymbol, bootstrapMethod};
}

template<typename MethodSignature, typename FT>
MethodBinding makeIntrinsicMethodBindingWithSignature(const std::string &intrinsicName, const std::string &selector, const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &argumentTypes, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto intrinsicSymbol = internSymbol(intrinsicName);
    auto selectorSymbol = internSymbol(selector);
    auto bootstrapMethod = std::make_shared<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->setMethodSignature(receiverType, resultType, argumentTypes);
    bootstrapMethod->setIntrinsicName(intrinsicSymbol);
    bootstrapMethod->addMethodFlags(flags);
    bootstrapMethod->registerInCurrentModule();
    return MethodBinding{selectorSymbol, bootstrapMethod};
}

template<typename FT>
MethodBinding makeIntrinsicMethodBindingWithSignature(const std::string &intrinsicName, const std::string &selector, const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &argumentTypes, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto intrinsicSymbol = internSymbol(intrinsicName);
    auto selectorSymbol = internSymbol(selector);
    auto bootstrapMethod = std::make_shared<BootstrapMethod<FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->setMethodSignature(receiverType, resultType, argumentTypes);
    bootstrapMethod->setIntrinsicName(intrinsicSymbol);
    bootstrapMethod->addMethodFlags(flags);
    bootstrapMethod->registerInCurrentModule();
    return MethodBinding{selectorSymbol, bootstrapMethod};
}

template<typename MethodSignature, typename FT>
SpecificMethodPtr makeConstructor(FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto selectorSymbol = internSymbol("()");
    auto bootstrapMethod = std::make_shared<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->makeConstructor();
    bootstrapMethod->addMethodFlags(flags);
    bootstrapMethod->registerInCurrentModule();
    return bootstrapMethod;
}
template<typename MethodSignature, typename FT>
SpecificMethodPtr makeIntrinsicConstructor(const std::string &intrinsicName, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto ctor = makeConstructor<MethodSignature> (std::forward<FT> (functor), flags);
    ctor->setIntrinsicName(internSymbol(intrinsicName));
    return ctor;
}

template<typename FT>
SpecificMethodPtr makeConstructor(FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto selectorSymbol = internSymbol("()");
    auto bootstrapMethod = std::make_shared<BootstrapMethod<FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->makeConstructor();
    bootstrapMethod->addMethodFlags(flags);
    bootstrapMethod->registerInCurrentModule();
    return bootstrapMethod;
}

template<typename FT>
SpecificMethodPtr makeIntrinsicConstructor(const std::string &intrinsicName, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto ctor = makeConstructor<FT> (std::forward<FT> (functor), flags);
    ctor->setIntrinsicName(internSymbol(intrinsicName));
    return ctor;
}

template<typename MethodSignature, typename FT>
SpecificMethodPtr makeConversion(FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto selectorSymbol = internSymbol("conv");
    auto bootstrapMethod = std::make_shared<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->makeConversion();
    bootstrapMethod->addMethodFlags(flags);
    bootstrapMethod->registerInCurrentModule();
    return bootstrapMethod;
}
template<typename MethodSignature, typename FT>
SpecificMethodPtr makeIntrinsicConversion(const std::string &intrinsicName, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto ctor = makeConversion<MethodSignature> (std::forward<FT> (functor), flags);
    ctor->setIntrinsicName(internSymbol(intrinsicName));
    return ctor;
}

template<typename FT>
SpecificMethodPtr makeConversion(FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto selectorSymbol = internSymbol("()");
    auto bootstrapMethod = std::make_shared<BootstrapMethod<FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->makeConversion();
    bootstrapMethod->addMethodFlags(flags);
    bootstrapMethod->registerInCurrentModule();
    return bootstrapMethod;
}

template<typename FT>
SpecificMethodPtr makeIntrinsicConversion(const std::string &intrinsicName, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto ctor = makeConversion<FT> (std::forward<FT> (functor), flags);
    ctor->setIntrinsicName(internSymbol(intrinsicName));
    return ctor;
}

template<typename MethodSignature, typename FT>
SpecificMethodPtr makeConversionWithSignature(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &argumentTypes, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto selectorSymbol = internSymbol("conv");
    auto bootstrapMethod = std::make_shared<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->setMethodSignature(receiverType, resultType, argumentTypes);
    bootstrapMethod->makeConversion();
    bootstrapMethod->addMethodFlags(flags);
    bootstrapMethod->registerInCurrentModule();
    return bootstrapMethod;
}
template<typename MethodSignature, typename FT>
SpecificMethodPtr makeIntrinsicConversionWithSignature(const std::string &intrinsicName, const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &argumentTypes, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto ctor = makeConversionWithSignature<MethodSignature> (receiverType, resultType, argumentTypes, std::forward<FT> (functor), flags);
    ctor->setIntrinsicName(internSymbol(intrinsicName));
    return ctor;
}

} // End of namespace BootstrapEnvironment
} // End of namespace SysmelMoebius

#endif //SYSMEL_COMPILER_BOOTSTRAP_ENVIRONMENT_BOOTSTRAP_TYPE_HPP