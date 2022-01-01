#ifndef SYSMEL_ENVIRONMENT_BOOTSTRAP_METHOD_HPP
#define SYSMEL_ENVIRONMENT_BOOTSTRAP_METHOD_HPP
#pragma once

#include "SpecificMethod.hpp"
#include "Wrappers.hpp"
#include "ArgumentCountError.hpp"
#include "FunctionalType.hpp"
#include <type_traits>

namespace Sysmel
{
namespace Environment
{

SYSMEL_DECLARE_BOOTSTRAP_CLASS(BootstrapMethodBase);
SYSMEL_DECLARE_BOOTSTRAP_CLASS(SSAFunction);

/**
 * I am the base interface for a type that is specifically defined by the bootstrap environment.
 */
class SYSMEL_COMPILER_LIB_EXPORT BootstrapMethodBase : public SubtypeOf<SpecificMethod, BootstrapMethodBase>
{
public:
    static constexpr char const __typeName__[] = "BootstrapMethod";
    
    virtual bool isBootstrapMethod() const override;
    virtual bool isCompileTimeEvaluableMethod() const override;

    virtual SSAValuePtr asSSAValueRequiredInPosition(const ASTSourcePositionPtr &requiredSourcePosition) override;
    
protected:
    SSAFunctionPtr ssaCompiledFunction;
};

template<typename MethodSignature, typename FT = void>
class BootstrapMethod;

template<typename ResultType, typename ReceiverType, typename... Args, typename FT>
class BootstrapMethod<ResultType (ReceiverType, Args...), FT> : public BootstrapMethodBase
{
public:
    typedef BootstrapMethod<ResultType (Args...), FT> SelfType;

    BootstrapMethod(const AnyValuePtr &initialSelector, const FT &initialFunctor)
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
            auto error = basicMakeObject<ArgumentCountError> ();
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
            auto error = basicMakeObject<ArgumentCountError> ();
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
            auto error = basicMakeObject<ArgumentCountError> ();
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
BootstrapMethodBasePtr makeBootstrapMethod(const std::string &selector, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto selectorSymbol = internSymbol(selector);
    auto method = basicMakeObject<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    method->addMethodFlags(flags);
    return method;
}

template<typename MethodSignature, typename FT>
MethodBinding makeMethodBinding(const AnyValuePtr &selector, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto bootstrapMethod = basicMakeObject<BootstrapMethod<MethodSignature, FT> > (selector, std::forward<FT> (functor));
    bootstrapMethod->addMethodFlags(flags);
    return MethodBinding{selector, bootstrapMethod};
}

template<typename MethodSignature, typename FT>
MethodBinding makeMethodBinding(const std::string &selector, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    return makeMethodBinding<MethodSignature> (internSymbol(selector), std::forward<FT> (functor), flags);
}

template<typename FT>
MethodBinding makeMethodBinding(const AnyValuePtr &selector, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto bootstrapMethod = basicMakeObject<BootstrapMethod<FT> > (selector, std::forward<FT> (functor));
    bootstrapMethod->addMethodFlags(flags);
    return MethodBinding{selector, bootstrapMethod};
}

template<typename FT>
MethodBinding makeMethodBinding(const std::string &selector, FT &&functor, MethodFlags flags = MethodFlags::None)
{
   return makeMethodBinding(internSymbol(selector), std::forward<FT> (functor), flags);
}

template<typename MethodSignature, typename FT>
MethodBinding makeIntrinsicMethodBinding(const std::string &intrinsicName, const std::string &selector, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto intrinsicSymbol = internSymbol(intrinsicName);
    auto selectorSymbol = internSymbol(selector);
    auto bootstrapMethod = basicMakeObject<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->setIntrinsicName(intrinsicSymbol);
    bootstrapMethod->addMethodFlags(flags);
    return MethodBinding{selectorSymbol, bootstrapMethod};
}

template<typename FT>
MethodBinding makeIntrinsicMethodBinding(const std::string &intrinsicName, const std::string &selector, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto intrinsicSymbol = internSymbol(intrinsicName);
    auto selectorSymbol = internSymbol(selector);
    auto bootstrapMethod = basicMakeObject<BootstrapMethod<FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->setIntrinsicName(intrinsicSymbol);
    bootstrapMethod->addMethodFlags(flags);
    return MethodBinding{selectorSymbol, bootstrapMethod};
}

template<typename MethodSignature, typename FT>
MethodBinding makeIntrinsicMethodBindingWithSignature(const std::string &intrinsicName, const std::string &selector, const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &argumentTypes, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto intrinsicSymbol = internSymbol(intrinsicName);
    auto selectorSymbol = internSymbol(selector);
    auto bootstrapMethod = basicMakeObject<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->setMethodSignature(receiverType, resultType, argumentTypes);
    bootstrapMethod->setIntrinsicName(intrinsicSymbol);
    bootstrapMethod->addMethodFlags(flags);
    return MethodBinding{selectorSymbol, bootstrapMethod};
}

template<typename FT>
MethodBinding makeIntrinsicMethodBindingWithSignature(const std::string &intrinsicName, const std::string &selector, const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &argumentTypes, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto intrinsicSymbol = internSymbol(intrinsicName);
    auto selectorSymbol = internSymbol(selector);
    auto bootstrapMethod = basicMakeObject<BootstrapMethod<FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->setMethodSignature(receiverType, resultType, argumentTypes);
    bootstrapMethod->setIntrinsicName(intrinsicSymbol);
    bootstrapMethod->addMethodFlags(flags);
    return MethodBinding{selectorSymbol, bootstrapMethod};
}

template<typename MethodSignature, typename FT>
SpecificMethodPtr makeConstructor(FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto selectorSymbol = internSymbol("()");
    auto bootstrapMethod = basicMakeObject<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->makeConstructor();
    bootstrapMethod->addMethodFlags(flags);
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
    auto bootstrapMethod = basicMakeObject<BootstrapMethod<FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->makeConstructor();
    bootstrapMethod->addMethodFlags(flags);
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
SpecificMethodPtr makeConstructorWithSignature(const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &argumentTypes, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto selectorSymbol = internSymbol("()");
    auto bootstrapMethod = basicMakeObject<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->setMethodSignature(receiverType, resultType, argumentTypes);
    bootstrapMethod->makeConstructor();
    bootstrapMethod->addMethodFlags(flags);
    return bootstrapMethod;
}
template<typename MethodSignature, typename FT>
SpecificMethodPtr makeIntrinsicConstructorWithSignature(const std::string &intrinsicName, const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &argumentTypes, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto ctor = makeConstructorWithSignature<MethodSignature> (receiverType, resultType, argumentTypes, std::forward<FT> (functor), flags);
    ctor->setIntrinsicName(internSymbol(intrinsicName));
    return ctor;
}

template<typename MethodSignature, typename FT>
SpecificMethodPtr makeConversion(FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto selectorSymbol = internSymbol("conv");
    auto bootstrapMethod = basicMakeObject<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->makeConversion();
    bootstrapMethod->addMethodFlags(flags);
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
    auto bootstrapMethod = basicMakeObject<BootstrapMethod<FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->makeConversion();
    bootstrapMethod->addMethodFlags(flags);
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
    auto bootstrapMethod = basicMakeObject<BootstrapMethod<MethodSignature, FT> > (selectorSymbol, std::forward<FT> (functor));
    bootstrapMethod->setMethodSignature(receiverType, resultType, argumentTypes);
    bootstrapMethod->makeConversion();
    bootstrapMethod->addMethodFlags(flags);
    return bootstrapMethod;
}
template<typename MethodSignature, typename FT>
SpecificMethodPtr makeIntrinsicConversionWithSignature(const std::string &intrinsicName, const TypePtr &receiverType, const TypePtr &resultType, const TypePtrList &argumentTypes, FT &&functor, MethodFlags flags = MethodFlags::None)
{
    auto ctor = makeConversionWithSignature<MethodSignature> (receiverType, resultType, argumentTypes, std::forward<FT> (functor), flags);
    ctor->setIntrinsicName(internSymbol(intrinsicName));
    return ctor;
}

} // End of namespace Environment
} // End of namespace Sysmel

#endif //SYSMEL_ENVIRONMENT_BOOTSTRAP_TYPE_HPP